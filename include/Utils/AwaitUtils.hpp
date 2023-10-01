#pragma once

#include <tuple>
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"
#include "System/Threading/Thread.hpp"
#include <functional>

namespace BedroomPartyLB::AwaitUtils
{
    template<typename... Targs>
    using AwaitableFunc = std::function<void(std::function<void(Targs...)>)>;

    template<typename... Targs>
    std::tuple<Targs...> AwaitValue(AwaitableFunc<Targs...> func, bool wasteOfTime = false)
    {
        std::tuple<Targs...>* tuple = new std::tuple<Targs...>();
        bool* inProgress = new bool(true);
        Lapiz::Utilities::MainThreadScheduler::Schedule([inProgress, tuple, func]()
        {
            func([inProgress, tuple](Targs... myValue)
            {
                *inProgress = false;
                *tuple = std::make_tuple(myValue...);
            });
        });
        while (*inProgress) System::Threading::Thread::Yield();
        std::tuple<Targs...> toReturn;
        toReturn.swap(*tuple);
        delete inProgress;
        delete tuple;
        return toReturn;
    }

    template <typename T>
    T AwaitValue(AwaitableFunc<T> func)
    {
        return std::get<T>(AwaitValue(func, true));
    }

    template<typename TRet, typename ...TArgs>
    using func_ptr = TRet(*)(TArgs...);

    template <typename T, typename TRet>
    T AwaitValue(func_ptr<TRet, std::function<void(T)>> func)
    {
        return AwaitValue(std::function(func));
    }

    template <typename TRet, typename... TArgs>
    std::tuple<TArgs...> AwaitValue(func_ptr<TRet, std::function<void(TArgs...)>> func)
    {
        return AwaitValue(std::function(func));
    }

    template<typename T, typename Bind>
    std::function<void(T)> MakeFunction(std::function<void(T)> func, Bind bind)
    {
        return std::function<void(T)>(bind);
    }

    template<typename T, typename... TArgs, typename Bind>
    auto MakeFunction(std::function<void(T, TArgs...)> func, Bind bind)
    {
        std::function<void(TArgs...)> f;
        return MakeFunction(f, bind);
    }

    template <typename Func, typename... TArgs>
    auto AwaitValue(Func func, TArgs... args)
    {
        return AwaitValue(MakeFunction(std::function(func), std::bind(func, args..., std::placeholders::_1)));
    }
}
