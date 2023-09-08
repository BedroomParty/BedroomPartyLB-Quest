#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/LeaderboardViewController.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel.hpp"
#include "PluginUI/LeaderboardViewController.hpp"
#include "UnityEngine/Resources.hpp"
#include "WebUtils.hpp"
#include "main.hpp"
#include "Hooks.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/LevelCompletionResultsHelper.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/PlayerLevelStatsData.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/PlayerData.hpp"
#include "GlobalNamespace/IReadonlyBeatmapData.hpp"
#include "GlobalNamespace/ScoreModel.hpp"
#include <string>
using namespace GlobalNamespace;

std::string sessionKey;

std::string GetModifiers(LevelCompletionResults *levelCompletionResults)
{
    std::string modifiers = "";
    auto levelModifiers = levelCompletionResults->gameplayModifiers;

    if (levelModifiers->noFailOn0Energy && levelCompletionResults->energy == 0)
        modifiers += "NF ";
    if (levelModifiers->songSpeed == GameplayModifiers::SongSpeed::Faster)
        modifiers += "FS ";
    if (levelModifiers->songSpeed == GameplayModifiers::SongSpeed::SuperFast)
        modifiers += "SF ";
    if (levelModifiers->songSpeed == GameplayModifiers::SongSpeed::Slower)
        modifiers += "SS ";
    if (levelModifiers->energyType == GameplayModifiers::EnergyType::Battery)
        modifiers += "BE ";
    if (levelModifiers->enabledObstacleType == GameplayModifiers::EnabledObstacleType::NoObstacles)
        modifiers += "NO ";
    if (levelModifiers->proMode)
        modifiers += "PM ";
    if (levelModifiers->instaFail)
        modifiers += "IF ";
    if (levelModifiers->failOnSaberClash)
        modifiers += "SC ";
    if (levelModifiers->noBombs)
        modifiers += "NB ";
    if (levelModifiers->strictAngles)
        modifiers += "SA ";
    if (levelModifiers->disappearingArrows)
        modifiers += "DA ";
    if (levelModifiers->ghostNotes)
        modifiers += "GN ";
    if (levelModifiers->smallCubes)
        modifiers += "SC";
    if (levelModifiers->noArrows)
        modifiers += "NA ";

    return modifiers;
}

MAKE_AUTO_HOOK_MATCH(LevelCompletionResultsHelper_ProcessScore, &LevelCompletionResultsHelper::ProcessScore, void, PlayerData * playerData, PlayerLevelStatsData * playerLevelStats,
LevelCompletionResults * levelCompletionResults, IReadonlyBeatmapData * transformedBeatmapData, IDifficultyBeatmap * difficultyBeatmap, PlatformLeaderboardsModel * platformLeaderboardsModel)
{
    LevelCompletionResultsHelper_ProcessScore(playerData, playerLevelStats, levelCompletionResults, transformedBeatmapData, difficultyBeatmap, platformLeaderboardsModel);
    if (levelCompletionResults->levelEndStateType == LevelCompletionResults::LevelEndStateType::Failed || levelCompletionResults->modifiedScore == 0 || levelCompletionResults->multipliedScore == 0)
        return;

    float maxScore = ScoreModel::ComputeMaxMultipliedScoreForBeatmap(transformedBeatmapData);
    float accuracy = levelCompletionResults->modifiedScore / maxScore * 100;

    std::string beatmapID = difficultyBeatmap->get_level()->i_IPreviewBeatmapLevel()->get_levelID()->Substring(13);
    std::string difficulty = std::to_string(difficultyBeatmap->get_difficultyRank());
    std::string characteristic = playerLevelStats->beatmapCharacteristic->serializedName;

    auto fc = levelCompletionResults->fullCombo;
    std::string fcBool; // When converting bools to strings C++ likes to turn it into an integer (0, 1), API doesn't like that
    if (fc)
        fcBool = "true";
    else
        fcBool = "false";
    auto modifiers = GetModifiers(levelCompletionResults);

    auto requestBody = "{"
    "\"difficulty\": " + difficulty + "," +
    "\"characteristic\": \"" + characteristic +"\"," +
    "\"id\": \"" + userID + "\"," +
    "\"multipliedScore\": " + std::to_string(levelCompletionResults->multipliedScore) + "," +
    "\"modifiedScore\": " + std::to_string(levelCompletionResults->modifiedScore) + "," +
    "\"accuracy\": " + std::to_string(accuracy) + "," +
    "\"misses\": " + std::to_string(levelCompletionResults->missedCount) + "," +
    "\"badCuts\": " + std::to_string(levelCompletionResults->badCutsCount) + "," +
    "\"fullCombo\": " + fcBool + "," +
    "\"modifiers\": \"" + modifiers + "\"" +
    "}";

    BedroomPartyLB::WebUtils::PostAsync(BASE_URL + "leaderboard/"+beatmapID+"/upload", requestBody, false, [difficultyBeatmap](std::string value, bool success) {
        if (success)
        {
            //DEBUG("{}", sessionKey);
            auto leaderboardView = UnityEngine::Resources::FindObjectsOfTypeAll<BedroomPartyLB::UI::LeaderboardViewController*>().FirstOrDefault();
            leaderboardView->RefreshLeaderboard(difficultyBeatmap);
        }
        else {
            DEBUG("Failed to send request!");
        }
    });
}