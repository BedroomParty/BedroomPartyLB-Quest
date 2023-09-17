#pragma once

#include "HMUI/ViewController.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/Selectable.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils-methods.hpp"
#include "custom-types/shared/macros.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/ModalView.hpp"
#include "bsml/shared/BSML/Components/CustomListTableData.hpp"
#include "PluginUI/SeasonList/SeasonListData.hpp"

DECLARE_CLASS_CODEGEN(BedroomPartyLB::UI, PanelViewController, HMUI::ViewController, 
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_INSTANCE_METHOD(void, OpenUserProfile);
    DECLARE_INSTANCE_METHOD(void, PostParse);
    DECLARE_INSTANCE_METHOD(void, SetBannerInfo);
    DECLARE_INSTANCE_METHOD(void, OpenWebsite);
    DECLARE_INSTANCE_METHOD(void, BPLogoClick);
    DECLARE_INSTANCE_METHOD(UnityEngine::Material*, GetRoundEdgeMaterial);
    DECLARE_INSTANCE_METHOD(void, SetSeasons, int currentSeason);
    DECLARE_INSTANCE_METHOD(void, OnPlayerUsernameClick);
    DECLARE_INSTANCE_METHOD(void, OnSeasonTextClick);
    DECLARE_INSTANCE_METHOD(void, HandleShittyListBollocksCunt);
    DECLARE_INSTANCE_METHOD(void, SetPrompt, StringW text, int time);

    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, prompt_loader);
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, playerAvatarLoading);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, prompt_text);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, playerUsername);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, playerAvatar);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, brLogo);
    DECLARE_INSTANCE_FIELD(HMUI::ModalView*, infoModal);
    DECLARE_INSTANCE_FIELD(HMUI::ModalView*, seasonSelectModal);
    DECLARE_INSTANCE_FIELD(BSML::CustomListTableData*, placeholderList);
    DECLARE_INSTANCE_FIELD(BedroomPartyLB::UI::SeasonListData*, seasonList);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, seasonText);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, seasonDescription);
    DECLARE_INSTANCE_FIELD(int, currentSeason);
)