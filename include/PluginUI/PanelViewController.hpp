#pragma once

#include "HMUI/ViewController.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/Selectable.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils-methods.hpp"
#include "custom-types/shared/macros.hpp"
#include "HMUI/ImageView.hpp"

DECLARE_CLASS_CODEGEN(BedroomPartyLB::UI, PanelViewController, HMUI::ViewController, 
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_METHOD(void, OpenUserProfile);
    DECLARE_INSTANCE_METHOD(void, PostParse);
    DECLARE_INSTANCE_METHOD(void, SetBannerInfo);


    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, prompt_loader);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, playerAvatarLoading);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, prompt_text);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, playerUsername);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, playerAvatar);
)