#pragma once

#include <functional>
#include "rapidjson-macros/shared/macros.hpp"
#include <string>

namespace BedroomPartyLB::AuthUtils
{
    enum AuthState
    {
        NOT_AUTHED,
        AUTHING,
        AUTHED,
        ERROR
    };
    extern AuthState authState;
    void AuthenticateUserAsync(std::function<void(AuthState)> callback);
    void RequestNewSessionKey(std::function<void(bool)> callback);

    DECLARE_JSON_CLASS(AuthBody, 
        NAMED_VALUE(std::string, id, "id");
        NAMED_VALUE(std::string, gameVersion, "gameVersion");
        NAMED_VALUE(std::string, pluginVersion, "pluginVersion");

        AuthBody(std::string id, std::string gameVersion, std::string pluginVersion) :
                id(id), gameVersion(gameVersion), pluginVersion(pluginVersion) {}

        public:
        std::string toString() const
        {
            return WriteToString(*this);
        }  
    )
}