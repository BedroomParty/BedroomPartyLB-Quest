#pragma once

#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "Models/LeaderboardData.hpp"

namespace BedroomPartyLB::Downloaders
{

    void DownloadLeaderboardAsync(GlobalNamespace::IDifficultyBeatmap* beatmap, int page, int scope, std::function<void(std::optional<Models::BPLeaderboard>,std::string)> callback);
}