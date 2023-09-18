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
    std::string fileText = readfile(Constants::SCARY_FILE);
    if (fileText.empty()) return false;
    std::string content;
    macaron::Base64::Decode(fileText, content);
    localPlayerInfo.apiKey = content.substr(0, content.find(','));
    localPlayerInfo.userID = content.substr(content.find(",") + 1, content.length());
    return true;
}

MAKE_AUTO_HOOK_MATCH(PlatformLeaderboardViewController_DidActivate, &PlatformLeaderboardViewController::DidActivate, void, PlatformLeaderboardViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling){
    PlatformLeaderboardViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);
    if (!firstActivation) return;
    if (!ReadScary())
    {
        AuthUtils::authState = AuthUtils::ERROR;
        return leaderboard.get_panelViewController()->SetBannerInfo();
    }
    AuthUtils::AuthenticateUserAsync([](AuthUtils::AuthState state)
    {
        AuthUtils::authState = state;
        leaderboard.get_leaderboardViewController()->RefreshLeaderboard(leaderboard.currentDifficultyBeatmap);
        leaderboard.get_panelViewController()->SetBannerInfo();
    });
}