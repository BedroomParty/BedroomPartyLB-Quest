#pragma once

#include <functional>
#include <string>

namespace BedroomPartyLB::AuthUtils{
    enum AuthState {
        NOT_AUTHED,
        AUTHING,
        AUTHED,
        ERROR
    };
    extern AuthState authState;
    void AuthenticateUserAsync(std::function<void(AuthState, std::string)> callback);
}