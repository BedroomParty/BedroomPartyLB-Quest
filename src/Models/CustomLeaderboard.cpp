#include "Models/CustomLeaderboard.hpp"
#include "PluginUI/LeaderboardViewController.hpp"
#include "PluginUI/PanelViewController.hpp"
#include "bsml/shared/Helpers/creation.hpp"

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
}