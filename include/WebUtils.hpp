#pragma once

#include "custom-types/shared/coroutine.hpp"
namespace BedroomPartyLB::WebUtils
{
    void PostAsync(std::string url, std::string body, bool isLogin, std::function<void(std::string, bool)> callback);
    void GetAsync(std::string url, std::function<void(std::string, bool)> callback);

    /*StringW TestServer();
    StringW GetLeaderboard(StringW hash, int difficulty, StringW characteristic, StringW sort, long userID);
    void UserLogin();
    StringW GetUsername(std::function<void(StringW)> callback);*/
}