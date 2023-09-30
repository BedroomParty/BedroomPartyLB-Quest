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
}

