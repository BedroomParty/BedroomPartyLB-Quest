#pragma once

#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/delegate.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/AsyncOperation.hpp"
#include "System/Action_1.hpp"
#include "System/Net/HttpStatusCode.hpp"

namespace BedroomPartyLB::WebUtils
{
    void PostAsync(std::string url, std::string body, bool isLogin, std::function<void(std::string, bool, int)> callback);
    void GetAsync(std::string url, std::function<void(std::string, bool)> callback);
    void GetImageAsync(std::string URL, std::function<void(UnityEngine::Sprite*)> callback);
}