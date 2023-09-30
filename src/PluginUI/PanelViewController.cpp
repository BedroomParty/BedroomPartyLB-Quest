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
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/VerticalScrollIndicator.hpp"
#include "Utils/TweeningUtils.hpp"
#include "PluginUI/TextHoverEffect.hpp"
#include "PluginUI/RainbowAnimation.hpp"
#include "Utils/CoroUtils.hpp"
#include "UnityEngine/WaitForSeconds.hpp"

using namespace System::Collections;
using namespace UnityEngine;

DEFINE_TYPE(BedroomPartyLB::UI, PanelViewController);
namespace BedroomPartyLB::UI
{
    void PanelViewController::HandleShittyListBollocksCunt()
    {
        auto placeholderTableView = placeholderList->tableView;
        auto searchGo = placeholderList->get_gameObject();
        Object::DestroyImmediate(placeholderList);
        seasonList = nullptr;
        seasonList = searchGo->AddComponent<SeasonListData*>();
        seasonList->tableView = placeholderTableView;
        placeholderTableView->SetDataSource(seasonList->i_IDataSource(), false);
        seasonList->set_name("SeasonList");
    }

    UnityEngine::Material* PanelViewController::GetRoundEdgeMaterial()
    {
        static SafePtrUnity<Material> roundEdgeMaterial;
        if (roundEdgeMaterial) return roundEdgeMaterial.ptr();
        roundEdgeMaterial = Resources::FindObjectsOfTypeAll<Material *>().First([](auto &v){ return v->get_name() == "UINoGlowRoundEdge"; });
        return roundEdgeMaterial.ptr();
    }

    void PanelViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (!firstActivation) return;
        BSML::parse_and_construct(IncludedAssets::PanelView_bsml, this->get_transform(), this);
        BSML::parse_and_construct(IncludedAssets::infoModal_bsml, leaderboard.get_leaderboardViewController()->get_transform(), this);
        BSML::parse_and_construct(IncludedAssets::seasonSelectModal_bsml, leaderboard.get_leaderboardViewController()->get_transform(), this);
        HandleShittyListBollocksCunt();
        using namespace AuthUtils;
        if ((authState == AUTHED || authState == ERROR) && playerAvatarLoading->get_gameObject()->get_activeSelf()) SetBannerInfo();
        TextHoverEffect::AddEffect(playerUsername, TMPro::FontStyles::Underline, TMPro::FontStyles::Normal);
    }

    void PanelViewController::OpenUserProfile()
    {
        Application::OpenURL(Constants::USER_PROFILE_LINK + localPlayerInfo.userID);
    }

    void PanelViewController::BPLogoClick()
    {
        infoModal->Show(true, true, nullptr);
    }

    void PanelViewController::OpenWebsite()
    {
        Application::OpenURL("https://thebedroom.party");
    }

    void PanelViewController::PostParse()
    {
        prompt_loader->SetActive(true);
        prompt_text->get_gameObject()->SetActive(true);
        SetPrompt("Authenticating...", -1, true);
    }

    void PanelViewController::SetSeasons(int currentSeason)
    {
        this->currentSeason = currentSeason;
        Sprite* placeholderSprite = BSML::Utilities::LoadSpriteRaw(IncludedAssets::Logo_png);
        seasonList->seasonList.reserve(currentSeason + 5);
        for (int i = 0; i < currentSeason; i++)
        {
            if (currentSeason - i == currentSeason)
                seasonList->seasonList.emplace_back(currentSeason, "Speed Tech", 1, 1234, placeholderSprite);
            else
                seasonList->seasonList.emplace_back(currentSeason - i, "No Pauses", 14, 43, placeholderSprite);
        }
        // seasonText->SetText("Season " + std::to_string(seasonList->seasonList[0].seasonNumber));
        seasonDescription->SetText(seasonList->seasonList[0].seasonDescription);
        seasonList->tableView->ReloadData();
    }

    void PanelViewController::OnPlayerUsernameClick()
    {
        if (localPlayerInfo.userID.empty()) return;
        Application::OpenURL(Constants::USER_PROFILE_LINK + localPlayerInfo.userID);
    }

    void PanelViewController::OnSeasonTextClick()
    {
        seasonSelectModal->Show(true, true, nullptr);
    }

    void PanelViewController::SetBannerInfo()
    {
        if (!this->isActivated || !this->wasActivatedBefore) return;
        if (AuthUtils::authState != AuthUtils::AUTHED && AuthUtils::authState != AuthUtils::ERROR) return;
        bool error = AuthUtils::authState == AuthUtils::ERROR;
        SetPrompt((error ? "<color=#f0584a>Authentication Failed!</color>" : "<color=#43e03a>Successfully Authenticated!</color>"), 5);
        prompt_loader->set_active(false);
        playerUsername->SetText(error ? "failed" : localPlayerInfo.username);
        if (!error) BSML::Utilities::SetImage(playerAvatar, string_format("%suser/%s/avatar", Constants::BASE_URL.c_str(), localPlayerInfo.userID.c_str()));
        playerAvatar->set_material(GetRoundEdgeMaterial());
        playerAvatarLoading->SetActive(false);
        if (error) return;
        SetSeasons(15); 
        leaderboard.get_bedroomPartyStaffAsync([this](std::vector<std::string> staff)
        {
            if (std::find(staff.begin(), staff.end(), localPlayerInfo.userID) != staff.end())
            {
                playerUsername->get_gameObject()->AddComponent<RainbowAnimation*>();
            } 
        });
    }

    void PanelViewController::SetPrompt(StringW text, int time, bool loader)
    {
        if (!prompt_text) return;
        prompt_text->SetText(text);
        prompt_loader->set_active(loader);
        TweeningUtils::FadeText(prompt_text, true, 0.2f);
        currentPromptId = System::Guid::NewGuid().ToString();
        if (time < 0) return;
        std::string guid = currentPromptId;
        CoroUtils::RunCoroutine([=]() -> custom_types::Helpers::Coroutine
        {
            std::string localGuid = std::move(guid);
            co_yield reinterpret_cast<IEnumerator*>(WaitForSeconds::New_ctor(time));
            if (currentPromptId != localGuid) co_return;
            TweeningUtils::FadeText(prompt_text, false, 0.15f);
            prompt_loader->set_active(false);
            co_return;
        });
    }

    StringW PanelViewController::get_currentVersion()
    {
        std::string gameVersion = StringUtils::split(UnityEngine::Application::get_version(), '_')[0];
        return string_format("You are running BPLB v%s on Beat Saber v%s", modInfo.version.c_str(), gameVersion.c_str());
    }
}