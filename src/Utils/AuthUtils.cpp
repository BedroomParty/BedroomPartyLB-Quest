#include "Utils/AuthUtils.hpp"
#include "Utils/WebUtils.hpp"
#include "main.hpp"
#include "Models/LocalPlayerInfo.hpp"
#include "Utils/Constants.hpp"

namespace BedroomPartyLB::AuthUtils{
    AuthState authState = AuthState::NOT_AUTHED;
    void AuthenticateUserAsync(std::function<void(AuthState, std::string)> callback){
        authState = AuthState::AUTHING;
        WebUtils::PostAsync(Constants::BASE_URL + "user/login", "{ \"id\": \"" + localPlayerInfo.userID +"\" }", true, [callback](std::string value, bool success) {
            if (success)
            {
                rapidjson::Document doc;
                doc.Parse(value.c_str());
                if (doc.HasMember("sessionKey")) return callback(AuthState::AUTHED, doc["sessionKey"].GetString());
                else return callback(AuthState::ERROR, "");
            }
            callback(AuthState::ERROR, "");
        });
    }
}