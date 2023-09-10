#include "Hooks.hpp"
#include "GlobalNamespace/PlatformLeaderboardViewController.hpp"
#include "modloader/shared/modloader.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/AuthUtils.hpp"
#include "Utils/Constants.hpp"
#include "logging.hpp"
#include "Models/CustomLeaderboard.hpp"
#include "Utils/WebUtils.hpp"
#include "Models/LocalPlayerInfo.hpp"
#include "Base64.h"
#include "main.hpp"

using namespace GlobalNamespace;
using namespace BedroomPartyLB;

extern Models::CustomLeaderboard leaderboard;

bool ReadScary()
    {
        INFO("Reading scary file");
        std::string fileText = readfile(Constants::SCARY_FILE);
        if (fileText.empty()) return false;
        std::string content;
        macaron::Base64::Decode(fileText, content);
        INFO("Read scary file");
        localPlayerInfo.apiKey = content.substr(0, content.find(','));
        INFO("Substring 1");
        localPlayerInfo.userID = content.substr(content.find(",") + 1, content.length());
        DEBUG("apiKey, {} \nuserID: {}", localPlayerInfo.apiKey, localPlayerInfo.userID);
        return true;
    }

MAKE_AUTO_HOOK_MATCH(PlatformLeaderboardViewController_DidActivate, &PlatformLeaderboardViewController::DidActivate, void, PlatformLeaderboardViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    PlatformLeaderboardViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (!firstActivation) return;
    if (!ReadScary()){
        AuthUtils::authState = AuthUtils::ERROR;
        return leaderboard.get_panelViewController()->SetBannerInfo();
    }
    WebUtils::GetAsync(Constants::BASE_URL + "user/" + localPlayerInfo.userID, [](std::string value, bool success) {
            if (success)
            {
                rapidjson::Document doc;
                doc.Parse(value.c_str());
                if (doc.HasMember("username") && doc.HasMember("avatar"))
                {
                    localPlayerInfo.username = doc["username"].GetString();
                    localPlayerInfo.avatarLink = doc["avatar"].GetString();
                }
                else localPlayerInfo.username = "null";
            }
            leaderboard.get_panelViewController()->SetBannerInfo();
        });

        AuthUtils::AuthenticateUserAsync([](AuthUtils::AuthState state, std::string sessionKey){
            AuthUtils::authState = state;
            localPlayerInfo.sessionKey = sessionKey;
            leaderboard.get_leaderboardViewController()->RefreshLeaderboard(leaderboard.currentDifficultyBeatmap);
            leaderboard.get_panelViewController()->SetBannerInfo();
        });
}