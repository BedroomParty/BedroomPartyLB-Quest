#pragma once

#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/delegate.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/AsyncOperation.hpp"
#include "System/Action_1.hpp"

typedef System::Action_1<UnityEngine::AsyncOperation*>* DLFinish;
#define DLCompletedDeleg(Func) custom_types::MakeDelegate<DLFinish>(classof(DLFinish), static_cast<std::function<void(UnityEngine::AsyncOperation*)>>(Func)) \

namespace BedroomPartyLB::WebUtils
{
    void PostAsync(std::string url, std::string body, bool isLogin, std::function<void(std::string, bool)> callback);
    void GetAsync(std::string url, std::function<void(std::string, bool)> callback);
    void GetImageAsync(std::string URL, std::function<void(UnityEngine::Sprite*)> callback);

    /*StringW TestServer();
    StringW GetLeaderboard(StringW hash, int difficulty, StringW characteristic, StringW sort, long userID);
    void UserLogin();
    StringW GetUsername(std::function<void(StringW)> callback);*/
}