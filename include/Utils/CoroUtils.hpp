#pragma once

#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

namespace BedroomPartyLB::CoroUtils
{
    inline void RunCoroutine(UnityEngine::MonoBehaviour* parent, custom_types::Helpers::CoroFuncType coroutine)
    {
        parent->StartCoroutine(custom_types::Helpers::new_coro(coroutine()));
    }

    inline void RunCoroutine(custom_types::Helpers::CoroFuncType coroutine)
    {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::new_coro(coroutine()));
    }

    template<typename M = UnityEngine::MonoBehaviour, typename... TArgs>
    inline void RunCoroutine(M* parent, std::function<custom_types::Helpers::Coroutine(TArgs...)> coroutine, TArgs... values)
    {
        parent->StartCoroutine(custom_types::Helpers::new_coro(coroutine(values...)));
    }

    template<typename... TArgs>
    inline void RunCoroutine(std::function<custom_types::Helpers::Coroutine(TArgs...)> coroutine, TArgs... values)
    {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::new_coro(coroutine(values...)));
    }

    template<typename M = UnityEngine::MonoBehaviour, typename F, typename... TArgs>
    inline void RunCoroutine(M* parent, F coroutine, TArgs... values)
    {
        auto func = std::function<custom_types::Helpers::Coroutine(TArgs...)>(coroutine);
        parent->StartCoroutine(custom_types::Helpers::new_coro(func(values...)));
    }

    template<typename F, typename... TArgs>
    inline void RunCoroutine(F coroutine, TArgs... values)
    {
        auto func = std::function<custom_types::Helpers::Coroutine(TArgs...)>(coroutine);
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::new_coro(func(values...)));
    }
}