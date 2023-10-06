#include "Models/ExtraSongData.hpp"
#include "Hooks.hpp"
#include "GlobalNamespace/FlyingScoreEffect.hpp"
#include "GlobalNamespace/CutScoreBuffer.hpp"
#include "GlobalNamespace/NoteCutInfo.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/PauseController.hpp"
#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/PlayerSpecificSettings.hpp"
#include "GlobalNamespace/PracticeSettings.hpp"
#include "GlobalNamespace/ISaberSwingRatingCounter.hpp"
#include "GlobalNamespace/EnvironmentInfoSO.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/MainSettingsModelSO.hpp"
#include "GlobalNamespace/BeatmapDataCache.hpp"
#include "main.hpp"
#include "GlobalNamespace/GamePause.hpp"
#include "Models/CustomLeaderboard.hpp"
#include "GlobalNamespace/NoteCutInfo.hpp"
#include "GlobalNamespace/NoteData.hpp"

BedroomPartyLB::Models::ExtraSongData extraSongData;
using namespace GlobalNamespace;

int currentPerfectHits = 0;

MAKE_AUTO_HOOK_FIND_CLASS_UNSAFE_INSTANCE(GameplayCoreSceneSetupData_ctor, "", "GameplayCoreSceneSetupData", ".ctor", void, GameplayCoreSceneSetupData* self, IDifficultyBeatmap* difficultyBeatmap, IPreviewBeatmapLevel* previewBeatmapLevel, GameplayModifiers* gameplayModifiers, PlayerSpecificSettings* playerSpecificSettings, PracticeSettings* practiceSettings, bool useTestNoteCutSoundEffects, EnvironmentInfoSO* environmentInfo, ColorScheme* colorScheme, MainSettingsModelSO* mainSettingsModel, BeatmapDataCache* beatmapDataCache)
{
    GameplayCoreSceneSetupData_ctor(self, difficultyBeatmap, previewBeatmapLevel, gameplayModifiers, playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects, environmentInfo, colorScheme, mainSettingsModel, beatmapDataCache);
    extraSongData.reset();
    currentPerfectHits = 0;
    leaderboard.close_modals();
}

MAKE_AUTO_HOOK_MATCH(Restartbutton, &PauseMenuManager::RestartButtonPressed, void, PauseMenuManager* self)
{
    Restartbutton(self);
    extraSongData.reset();
    currentPerfectHits = 0;
}

MAKE_AUTO_HOOK_MATCH(HandleCutFinish, &CutScoreBuffer::HandleSaberSwingRatingCounterDidFinish, void, CutScoreBuffer* self, ISaberSwingRatingCounter* counter)
{
    HandleCutFinish(self, counter);
    if (self->noteCutInfo.noteData->colorType == ColorType::ColorA)
    {
        extraSongData.leftHandAverageScore.push_back(self->get_cutScore());
        extraSongData.leftHandTimeDependency.push_back(std::abs(self->noteCutInfo.cutNormal.z));
        extraSongData.totalBlocksHit.emplace_back(self->get_cutScore(), self->get_noteCutInfo().noteData->get_scoringType());
    }
    else if (self->noteCutInfo.noteData->colorType == ColorType::ColorB)
    {
        extraSongData.rightHandAverageScore.push_back(self->get_cutScore());
        extraSongData.rightHandTimeDependency.push_back(std::abs(self->noteCutInfo.cutNormal.z));
        extraSongData.totalBlocksHit.emplace_back(self->get_cutScore(), self->get_noteCutInfo().noteData->get_scoringType());
    }
    if (self->get_cutScore() == 115)
    {
        currentPerfectHits++;
        if (currentPerfectHits > extraSongData.perfectStreak) extraSongData.perfectStreak = currentPerfectHits;     
    }
    else currentPerfectHits = 0;
}

MAKE_AUTO_HOOK_MATCH(Pause, &GamePause::Pause, void, GamePause* self)
{
    Pause(self);
    extraSongData.pauses++;
    getLogger().info("paused %i times", extraSongData.pauses);
}