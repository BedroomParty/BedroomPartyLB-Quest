#include "PluginUI/PanelViewController.hpp"
#include "HMUI/ImageView.hpp"
#include "UnityEngine/Material.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/rapidjson.h"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "bsml/shared/Helpers/utilities.hpp"
#include "bsml/shared/BSML.hpp"
#include "UnityEngine/Application.hpp"
#include "UnityEngine/Resources.hpp"
#include "assets.hpp"
#include "WebUtils.hpp"
#include "logging.hpp"
#include <functional>
#include "main.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

DEFINE_TYPE(BedroomPartyLB::UI, PanelViewController);

namespace BedroomPartyLB::UI
{
    void ReadScary()
    {
        auto file = readfile("/sdcard/ModData/com.beatgames.beatsaber/Mods/BPLB/DO_NOT_SHARE.scary");
        //DEBUG("File contents: {}", file);
        apiKey = file.substr(0, file.find(','));
        userID = file.substr(file.find(","), file.size());
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
        //if (apiKey.empty())
        //    ReadScary();

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

        WebUtils::PostAsync(BASE_URL + "user/login", "{ \"id\": \"" + userID +"\" }", true, [this](std::string value, bool success) {
            if (success)
            {
                rapidjson::Document doc;
                doc.Parse(value.c_str());
                if (doc.HasMember("sessionKey"))
                {
                    sessionKey = doc["sessionKey"].GetString();
                    isAuthed = true;
                    prompt_text->SetText("");
                    prompt_loader->SetActive(false);
                }
            }
        });
    }
}