#pragma once

#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/IReadonlyBeatmapData.hpp"
#include "GlobalNamespace/VRController.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/PauseMenuManager.hpp"
#include "System/IDisposable.hpp"
#include "Zenject/IInitializable.hpp"
#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"
#include "GlobalNamespace/ComboUIController.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include <vector>
#include "HMUI/ModalView.hpp"
#include "HMUI/ImageView.hpp"
#include "UnityEngine/GameObject.hpp"
#include "bsml/shared/BSML/Components/ClickableText.hpp"
#include "Models/LeaderboardData.hpp"

DECLARE_CLASS_CODEGEN(BedroomPartyLB::UI, ScoreInfoModal, Il2CppObject,
    DECLARE_INSTANCE_FIELD(HMUI::ModalView*, scoreInfo);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, profileImage);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, profileImageLoading);
    DECLARE_INSTANCE_FIELD(BSML::ClickableText*, usernameScoreText);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, dateScoreText);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, ppScoreText);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, accScoreText);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, scoreScoreText);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, modifiersScoreText);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, fcScoreText);

    DECLARE_DEFAULT_CTOR();

    DECLARE_INSTANCE_METHOD(void, OnUserNameTextClick);
    DECLARE_INSTANCE_METHOD(void, PostParse);

    public:
    void setScoreModalText(BedroomPartyLB::Models::BPLeaderboardEntry entry, int index);

    private:
    BedroomPartyLB::Models::BPLeaderboardEntry currentEntry;
    const int scoreDetails = 4;
    const float infoFontSize = 4.2f;
    )