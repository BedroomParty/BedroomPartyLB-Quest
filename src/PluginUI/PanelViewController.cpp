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

DEFINE_TYPE(BedroomPartyLB::UI, PanelViewController);

std::string apiKey;
std::string userID;
extern BedroomPartyLB::Models::CustomLeaderboard leaderboard;
namespace BedroomPartyLB::UI
{
    void ReadScary()
    {
        INFO("Reading scary file");
        std::string content;
        macaron::Base64::Decode(readfile("/sdcard/ModData/com.beatgames.beatsaber/Mods/BPLB/DO_NOT_SHARE.scary"), content);
        INFO("Read scary file");
        apiKey = content.substr(0, content.find(','));
        INFO("Substring 1");
        userID = content.substr(content.find(",") + 1, content.length());
        DEBUG("apiKey, {} \nuserID: {}", apiKey, userID);
    }

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
        ReadScary();
        BSML::parse_and_construct(IncludedAssets::PanelView_bsml, this->get_transform(), this);
    }

    void PanelViewController::OpenUserProfile() {
        UnityEngine::Application::OpenURL("https://thebedroom.party/?user=" + userID);
    }

    void PanelViewController::PostParse() 
    {
        prompt_loader->SetActive(true);
        prompt_text->get_gameObject()->SetActive(true);
        prompt_text->SetText("Authenticating...");
        
        WebUtils::GetAsync(BASE_URL + "user/" + userID, [this](std::string value, bool success) {
            if (success)
            {
                rapidjson::Document doc;
                doc.Parse(value.c_str());
                if (doc.HasMember("username") && doc.HasMember("avatar"))
                {
                    playerUsername->SetText(doc["username"].GetString());
                    BSML::Utilities::SetImage(playerAvatar, doc["avatar"].GetString());
                    playerAvatar->set_material(GetRoundEdgeMaterial());
                    playerAvatarLoading->SetActive(false);
                }
                else
                    playerUsername->SetText("null");
            }
        });

        AuthUtils::AuthenticateUserAsync([this](AuthUtils::AuthState state){
            AuthUtils::authState = state;
            prompt_text->SetText("");
            prompt_loader->SetActive(false);
            leaderboard.get_leaderboardViewController()->RefreshLeaderboard(leaderboard.currentDifficultyBeatmap);
        });
    }
}