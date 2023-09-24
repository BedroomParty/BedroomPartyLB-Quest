#include "Utils/AuthUtils.hpp"
#include "Utils/WebUtils.hpp"
#include "main.hpp"
#include "Models/LocalPlayerInfo.hpp"
#include "Utils/Constants.hpp"
#include "UnityEngine/Application.hpp"
#include "Utils/StringUtils.hpp"

namespace BedroomPartyLB::AuthUtils
{
    AuthState authState = AuthState::NOT_AUTHED;
    void AuthenticateUserAsync(std::function<void(AuthState)> callback)
    {
        authState = AuthState::AUTHING;

        std::string id = localPlayerInfo.userID;
        std::string gameVersion = "v" + StringUtils::split(UnityEngine::Application::get_version(), '_')[0];
        std::string modVersion = "Quest v" + modInfo.version;
        std::string authBody = AuthBody(id, gameVersion, modVersion).toString();
        
        WebUtils::PostAsync(Constants::BASE_URL + "user/login", authBody, true, [callback](std::string value, bool success, int responseCode) 
        {
            if (success)
            {
                ReadFromString(value, localPlayerInfo);
                return callback(AuthState::AUTHED);     
            }
            callback(AuthState::ERROR);
        });
    }

    void RequestNewSessionKey(std::function<void(bool)> callback)
    {
        AuthenticateUserAsync([callback](AuthState state)
        {
            authState = state;
            callback(state == AuthState::AUTHED);
        });
    }
}