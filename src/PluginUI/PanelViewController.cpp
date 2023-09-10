#include "PluginUI/PanelViewController.hpp"
#include "HMUI/ImageView.hpp"
#include "UnityEngine/Material.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
#include "bsml/shared/BSML.hpp"
#include "UnityEngine/Application.hpp"
#include "UnityEngine/Resources.hpp"
#include "assets.hpp"
#include "Utils/WebUtils.hpp"
#include "logging.hpp"
#include <functional>
#include "main.hpp"
#include "Base64.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Utils/AuthUtils.hpp"
#include "Models/CustomLeaderboard.hpp"
#include "Models/LocalPlayerInfo.hpp"
#include "Utils/Constants.hpp"

DEFINE_TYPE(BedroomPartyLB::UI, PanelViewController);

extern BedroomPartyLB::Models::CustomLeaderboard leaderboard;
namespace BedroomPartyLB::UI
{
    UnityEngine::Material *GetRoundEdgeMaterial()
    {
        UnityEngine::Material *roundEdgeMaterial = nullptr;
        auto materials = UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::Material*>();
        for (auto material : materials)
        {
            if (material->get_name()->Contains("UINoGlowRoundEdge"))
            {
                roundEdgeMaterial = material;
                break;
            }
        }
        return roundEdgeMaterial;
    }

    void PanelViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (!firstActivation) return;
        BSML::parse_and_construct(IncludedAssets::PanelView_bsml, this->get_transform(), this);
        infoModal->get_transform()->set_parent(leaderboard.get_leaderboardViewController()->get_transform());
    }

    void PanelViewController::OpenUserProfile() {
        UnityEngine::Application::OpenURL(Constants::USER_PROFILE_LINK + localPlayerInfo.userID);
    }

    void PanelViewController::BPLogoClick(){
        infoModal->Show(true, true, nullptr);
    }

    void PanelViewController::OpenWebsite() {
        UnityEngine::Application::OpenURL("https://thebedroom.party");
    }

    void PanelViewController::PostParse() 
    {
        prompt_loader->SetActive(true);
        prompt_text->get_gameObject()->SetActive(true);
        prompt_text->SetText("Authenticating...");
        SetBannerInfo();
    }

    void PanelViewController::SetBannerInfo(){
        if (!this->isActivated) return;
        if (AuthUtils::authState != AuthUtils::AUTHED && AuthUtils::authState != AuthUtils::ERROR) return;
        prompt_text->SetText("");
        prompt_loader->SetActive(false);
        playerUsername->SetText(localPlayerInfo.username);
        BSML::Utilities::SetImage(playerAvatar, localPlayerInfo.avatarLink);
        playerAvatar->set_material(GetRoundEdgeMaterial());
        playerAvatarLoading->SetActive(false);
    }
}