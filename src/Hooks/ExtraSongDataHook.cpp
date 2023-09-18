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

BedroomPartyLB::Models::ExtraSongData extraSongData;
using namespace GlobalNamespace;

int currentPerfectHits = 0;

MAKE_AUTO_HOOK_FIND_CLASS_UNSAFE_INSTANCE(GameplayCoreSceneSetupData_ctor, "", "GameplayCoreSceneSetupData", ".ctor", void, GameplayCoreSceneSetupData* self, IDifficultyBeatmap* difficultyBeatmap, IPreviewBeatmapLevel* previewBeatmapLevel, GameplayModifiers* gameplayModifiers, PlayerSpecificSettings* playerSpecificSettings, PracticeSettings* practiceSettings, bool useTestNoteCutSoundEffects, EnvironmentInfoSO* environmentInfo, ColorScheme* colorScheme, MainSettingsModelSO* mainSettingsModel, BeatmapDataCache* beatmapDataCache)
{
    GameplayCoreSceneSetupData_ctor(self, difficultyBeatmap, previewBeatmapLevel, gameplayModifiers, playerSpecificSettings, practiceSettings, useTestNoteCutSoundEffects, environmentInfo, colorScheme, mainSettingsModel, beatmapDataCache);
    extraSongData.reset();
    currentPerfectHits = 0;
}

MAKE_AUTO_HOOK_MATCH(HandleCutFinish, &CutScoreBuffer::HandleSaberSwingRatingCounterDidFinish, void, CutScoreBuffer* self, ISaberSwingRatingCounter* counter)
{
    HandleCutFinish(self, counter);
    if (self->noteCutInfo.noteData->colorType == ColorType::ColorA)
    {
        extraSongData.avgHandAccLeft.push_back(self->get_cutScore());
        extraSongData.avgHandTDLeft.push_back(std::abs(self->noteCutInfo.cutNormal.z));
    }
    else if (self->noteCutInfo.noteData->colorType == ColorType::ColorB)
    {
        extraSongData.avgHandAccRight.push_back(self->get_cutScore());
        extraSongData.avgHandTDRight.push_back(std::abs(self->noteCutInfo.cutNormal.z));
    }
    if (self->get_cutScore() == 115)
    {
        currentPerfectHits++;
        if (currentPerfectHits > extraSongData.perfectStreak) extraSongData.perfectStreak = currentPerfectHits;
        else currentPerfectHits = 0;       
    }
}

MAKE_AUTO_HOOK_MATCH(Pause, &PauseController::Pause, void, PauseController* self)
{
    Pause(self);
    extraSongData.pauses++;
}