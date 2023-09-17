#pragma once

#include "HMUI/ViewController.hpp"
#include "PluginUI/LeaderboardViewController.hpp"
#include "PluginUI/PanelViewController.hpp"
#include "leaderboardcore/shared/Models/CustomLeaderboard.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"

namespace BedroomPartyLB::Models
{
    class CustomLeaderboard final : public LeaderboardCore::Models::CustomLeaderboard
    {
        public:
        BedroomPartyLB::UI::PanelViewController *get_panelViewController() final override;
        BedroomPartyLB::UI::LeaderboardViewController *get_leaderboardViewController() final override;
        void get_bedroomPartyStaffAsync(std::function<void(std::vector<std::string>)> callback);
        GlobalNamespace::IDifficultyBeatmap* currentDifficultyBeatmap;

        private:
        SafePtrUnity<BedroomPartyLB::UI::PanelViewController> panelViewController;
        SafePtrUnity<BedroomPartyLB::UI::LeaderboardViewController> leaderboardViewController;
    };
}
extern BedroomPartyLB::Models::CustomLeaderboard leaderboard;