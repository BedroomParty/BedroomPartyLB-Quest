#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/IDifficultyBeatmapSet.hpp"
#include "Utils/WebUtils.hpp"
#include "main.hpp"
#include "Downloaders/LeaderboardDownloader.hpp"
#include "Models/LocalPlayerInfo.hpp"
#include "Utils/Constants.hpp"

namespace BedroomPartyLB::Downloaders
{
    void DownloadLeaderboardAsync(GlobalNamespace::IDifficultyBeatmap* beatmap, int page, int scope, std::function<void(std::optional<Models::BPLeaderboard>)> callback)
    {
        std::string beatmapID = beatmap->get_level()->i_IPreviewBeatmapLevel()->get_levelID()->Substring(13);
        std::string difficulty = std::to_string(beatmap->get_difficultyRank());
        std::string characteristic = beatmap->get_parentDifficultyBeatmapSet()->get_beatmapCharacteristic()->serializedName;
        std::string filter = scope == 0 ? "top" : "around";
        std::string url = Constants::BASE_URL + "leaderboard/"+beatmapID+"?char="+characteristic+"&diff="+difficulty+"&sort="+filter+"&limit=10&page="+std::to_string(page)+"&id="+localPlayerInfo.userID.c_str();
        getLogger().info("requesting leaderboard: %s", url.c_str());

        WebUtils::GetAsync(url, [callback](std::string value, bool success) 
        {
            if (!success || value == "Failed to find leaderboard") return callback({});
            Models::BPLeaderboard pageLeaderboard;
            ReadFromString(value, pageLeaderboard);
            callback(pageLeaderboard);
        });
    }
}