#include "Utils/AuthUtils.hpp"
#include "Utils/WebUtils.hpp"
#include "main.hpp"
#include "Models/LocalPlayerInfo.hpp"
#include "Utils/Constants.hpp"

namespace BedroomPartyLB::AuthUtils
{
    AuthState authState = AuthState::NOT_AUTHED;
    void AuthenticateUserAsync(std::function<void(AuthState)> callback)
    {
        authState = AuthState::AUTHING;
        WebUtils::PostAsync(Constants::BASE_URL + "user/login", "{ \"id\": \"" + localPlayerInfo.userID +"\" }", true, [callback](std::string value, bool success, int responseCode) 
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
        getLogger().info("requesting new key innit");
        AuthenticateUserAsync([callback](AuthState state)
        {
            authState = state;
            callback(state == AuthState::AUTHED);
        });
    }
}