#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/LeaderboardViewController.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel.hpp"
#include "PluginUI/LeaderboardViewController.hpp"
#include "UnityEngine/Resources.hpp"
#include "Utils/WebUtils.hpp"

#include "rapidjson-macros/shared/serialization.hpp"
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
#include "Models/CustomLeaderboard.hpp"
#include "Models/ScoreUploadBody.hpp"
#include "Models/LocalPlayerInfo.hpp"
#include "Utils/Constants.hpp"
#include "Utils/UploadUtils.hpp"
#include "Models/ExtraSongData.hpp"
#include "GlobalNamespace/PracticeViewController.hpp"
#include "UnityEngine/Resources.hpp"

using namespace GlobalNamespace;
using namespace BedroomPartyLB;

extern Models::CustomLeaderboard leaderboard;

std::string GetModifiers(LevelCompletionResults* levelCompletionResults)
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

MAKE_AUTO_HOOK_MATCH(LevelCompletionResultsHelper_ProcessScore, &LevelCompletionResultsHelper::ProcessScore, void, PlayerData* playerData, PlayerLevelStatsData* playerLevelStats, LevelCompletionResults* levelCompletionResults, IReadonlyBeatmapData* transformedBeatmapData, IDifficultyBeatmap* difficultyBeatmap, PlatformLeaderboardsModel* platformLeaderboardsModel)
{
    LevelCompletionResultsHelper_ProcessScore(playerData, playerLevelStats, levelCompletionResults, transformedBeatmapData, difficultyBeatmap, platformLeaderboardsModel);
    if (levelCompletionResults->levelEndStateType == LevelCompletionResults::LevelEndStateType::Failed) return;
    if (levelCompletionResults->modifiedScore == 0 || levelCompletionResults->multipliedScore == 0) return;
    if (!difficultyBeatmap->get_level()->i_IPreviewBeatmapLevel()->get_levelID()->Contains("custom")) return;
    float maxScore = ScoreModel::ComputeMaxMultipliedScoreForBeatmap(transformedBeatmapData);
    float accuracy = levelCompletionResults->modifiedScore / maxScore * 100;

    std::string beatmapID = difficultyBeatmap->get_level()->i_IPreviewBeatmapLevel()->get_levelID()->Substring(13);
    std::string characteristic = playerLevelStats->beatmapCharacteristic->serializedName;

    auto modifiers = GetModifiers(levelCompletionResults);
    int pauses = extraSongData.pauses;
    float accL = extraSongData.GetAverageFromList(extraSongData.avgHandAccLeft);
    float accR = extraSongData.GetAverageFromList(extraSongData.avgHandAccRight);
    float tdL = extraSongData.GetAverageFromList(extraSongData.avgHandTDLeft);
    float tdR = extraSongData.GetAverageFromList(extraSongData.avgHandTDRight);
    int streak = extraSongData.perfectStreak;
    float fcAcc = extraSongData.GetFcAcc();

    std::string requestBody = Models::ScoreUploadBody(difficultyBeatmap->get_difficultyRank(), characteristic, localPlayerInfo.userID,
                                                      levelCompletionResults->multipliedScore, levelCompletionResults->modifiedScore,
                                                      accuracy, levelCompletionResults->missedCount, levelCompletionResults->badCutsCount,
                                                      levelCompletionResults->fullCombo, modifiers, pauses, accL, accR, tdL, tdR, streak, fcAcc).toString();

    std::string url = Constants::BASE_URL + "leaderboard/" + beatmapID + "/upload";

    leaderboard.get_panelViewController()->SetPrompt("Uploading Score...", -1, true);

    std::thread(&UploadUtils::TryUploadScore, url, requestBody).detach();
}