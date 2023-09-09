#include "Utils/AuthUtils.hpp"
#include "Utils/WebUtils.hpp"
#include "main.hpp"

namespace BedroomPartyLB::AuthUtils{
    AuthState authState = AuthState::NOT_AUTHED;
    void AuthenticateUserAsync(std::function<void(AuthState)> callback){
        authState = AuthState::AUTHING;
        WebUtils::PostAsync(BASE_URL + "user/login", "{ \"id\": \"" + userID +"\" }", true, [callback](std::string value, bool success) {
            if (success)
            {
                rapidjson::Document doc;
                doc.Parse(value.c_str());
                if (doc.HasMember("sessionKey")) sessionKey = doc["sessionKey"].GetString();
                else return callback(AuthState::ERROR);
                return callback(AuthState::AUTHED);
            }
            callback(AuthState::ERROR);
        });
    }
}