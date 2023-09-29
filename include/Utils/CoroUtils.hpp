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
}