#pragma once

#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/LeaderboardTableView.hpp"
#include "HMUI/ViewController.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils-methods.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "bsml/shared/macros.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(BedroomPartyLB::UI, LeaderboardViewController, HMUI::ViewController, 
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_METHOD(void, PostParse);
    DECLARE_INSTANCE_METHOD(void, RefreshLeaderboard, GlobalNamespace::IDifficultyBeatmap *difficultyBeatmap);

    DECLARE_INSTANCE_FIELD(GlobalNamespace::LeaderboardTableView*, BPLeaderboard);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, errorText);
)