#pragma once

#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/LeaderboardTableView.hpp"
#include "HMUI/ViewController.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils-methods.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "bsml/shared/macros.hpp"
#include "custom-types/shared/macros.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"
#include "HMUI/IconSegmentedControl.hpp"
#include "HMUI/IconSegmentedControl_DataItem.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/GameObject.hpp"
#include "HMUI/ImageView.hpp"
#include "Models/LeaderboardData.hpp"
#include "GlobalNamespace/LeaderboardTableView_ScoreData.hpp"
#include "PluginUI/ScoreInfoModal.hpp"

DECLARE_CLASS_CODEGEN(BedroomPartyLB::UI, LeaderboardViewController, HMUI::ViewController, 
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_METHOD(void, PostParse);
    DECLARE_INSTANCE_METHOD(void, RefreshLeaderboard, GlobalNamespace::IDifficultyBeatmap *difficultyBeatmap);
    DECLARE_INSTANCE_METHOD(void, OnIconSelected, HMUI::IconSegmentedControl* segmentedControl, int index);
    DECLARE_INSTANCE_METHOD(void, OnPageUp);
    DECLARE_INSTANCE_METHOD(void, OnPageDown);
    DECLARE_INSTANCE_METHOD(void, OnBugClick);
    DECLARE_INSTANCE_METHOD(void, OnLBClick);

    DECLARE_INSTANCE_FIELD(HMUI::IconSegmentedControl*, scopeControl);
    DECLARE_INSTANCE_FIELD(GlobalNamespace::LeaderboardTableView*, BPLeaderboard);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, errorText);
    DECLARE_INSTANCE_FIELD(BSML::Backgroundable*, myHeader);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, up_button);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Button*, down_button);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, leaderboard_loading);
    DECLARE_INSTANCE_FIELD(BedroomPartyLB::UI::ScoreInfoModal*, scoreInfoModal);

    public:
    void ChangeScope();
    void CheckPage();
    int page;
    std::vector<HMUI::ImageView*> playerAvatars;
    std::vector<HMUI::ImageView*> avatarLoadings;
    private:
    void SetLoading(bool value, std::string error = "");
    void SetPlayerSprites(std::vector<BedroomPartyLB::Models::BPLeaderboardEntry> players, std::string refreshId);
    void AnnihilatePlayerSprites();
    void RichMyText(GlobalNamespace::LeaderboardTableView *tableView, std::vector<BedroomPartyLB::Models::BPLeaderboardEntry> entries);
    List<GlobalNamespace::LeaderboardTableView::ScoreData*>* CreateLeaderboardData(std::vector<Models::BPLeaderboardEntry> leaderboard);
)