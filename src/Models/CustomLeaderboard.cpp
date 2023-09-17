#include "Models/CustomLeaderboard.hpp"
#include "PluginUI/LeaderboardViewController.hpp"
#include "PluginUI/PanelViewController.hpp"
#include "bsml/shared/Helpers/creation.hpp"
#include "Utils/WebUtils.hpp"
#include "Utils/Constants.hpp"
#include "Utils/StringUtils.hpp"

namespace BedroomPartyLB::Models {
    BedroomPartyLB::UI::PanelViewController *CustomLeaderboard::get_panelViewController()
    {
        if (!panelViewController)
            panelViewController = BSML::Helpers::CreateViewController<BedroomPartyLB::UI::PanelViewController*>();
        return panelViewController.ptr();
    }

    BedroomPartyLB::UI::LeaderboardViewController *CustomLeaderboard::get_leaderboardViewController()
    {
        if (!leaderboardViewController)
            leaderboardViewController = BSML::Helpers::CreateViewController<BedroomPartyLB::UI::LeaderboardViewController*>();
        return leaderboardViewController.ptr();
    }

    void CustomLeaderboard::get_bedroomPartyStaffAsync(std::function<void(std::vector<std::string>)> callback){
        static std::vector<std::string> bedroomPartyStaff;
        if (!bedroomPartyStaff.empty()) return callback(bedroomPartyStaff);
        WebUtils::GetAsync(Constants::BASE_URL + "staff", [callback](std::string resonse, bool success){
            if (success) bedroomPartyStaff = StringUtils::split(resonse, ',');
            callback(bedroomPartyStaff);
        });
    }
}