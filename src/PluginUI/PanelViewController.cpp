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

DEFINE_TYPE(BedroomPartyLB::UI, PanelViewController);
namespace BedroomPartyLB::UI
{
    void PanelViewController::HandleShittyListBollocksCunt(){
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
        static SafePtrUnity<UnityEngine::Material> roundEdgeMaterial;
        if(roundEdgeMaterial) return roundEdgeMaterial.ptr();
        getLogger().info("so we made it here...");
        roundEdgeMaterial = UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::Material*>().First([](auto& v){return v->get_name() == "UINoGlowRoundEdge";});
        return roundEdgeMaterial.ptr();
    }

    void PanelViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (!firstActivation) return;
        BSML::parse_and_construct(IncludedAssets::PanelView_bsml, this->get_transform(), this);
        BSML::parse_and_construct(IncludedAssets::infoModal_bsml, leaderboard.get_leaderboardViewController()->get_transform(), this);
        BSML::parse_and_construct(IncludedAssets::seasonSelectModal_bsml, leaderboard.get_leaderboardViewController()->get_transform(), this);
        HandleShittyListBollocksCunt();
        if (AuthUtils::authState == AuthUtils::AUTHED && playerAvatarLoading->get_gameObject()->get_activeSelf()) SetBannerInfo();
        infoModal->get_gameObject()->get_transform()->set_parent(leaderboard.get_leaderboardViewController()->get_transform());
        seasonSelectModal->get_gameObject()->get_transform()->set_parent(leaderboard.get_leaderboardViewController()->get_transform());
        TextHoverEffect::AddEffect(playerUsername, TMPro::FontStyles::Underline, TMPro::FontStyles::Normal);
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
        SetPrompt("Authenticating...", -1);
    }

    void PanelViewController::SetSeasons(int currentSeason){
        this->currentSeason = currentSeason;
        UnityEngine::Sprite* placeholderSprite = BSML::Utilities::LoadSpriteRaw(IncludedAssets::Logo_png);
        seasonList->seasonList.reserve(currentSeason+5);
        for (int i=0; i<currentSeason; i++){
            getLogger().info("if there are destructor calls between these logs i am going to hurt a child");
            if (currentSeason - i == currentSeason)
                seasonList->seasonList.emplace_back(currentSeason, "Speed Tech", 1, 1234, placeholderSprite);
            else seasonList->seasonList.emplace_back(currentSeason - i, "No Pauses", 14, 43, placeholderSprite);
        }
        seasonText->SetText("Season " + std::to_string(seasonList->seasonList[0].seasonNumber));
        seasonDescription->SetText(seasonList->seasonList[0].seasonDescription);
        seasonList->tableView->ReloadData();
    }

    void PanelViewController::OnPlayerUsernameClick(){
        if (localPlayerInfo.userID.empty()) return;
        UnityEngine::Application::OpenURL("https://thebedroom.party/?user=" + localPlayerInfo.userID);
    }

    void PanelViewController::OnSeasonTextClick(){
        seasonSelectModal->Show(true, true, nullptr);
    }

    void PanelViewController::SetBannerInfo(){
        if (!this->isActivated || !this->wasActivatedBefore) return;
        if (AuthUtils::authState != AuthUtils::AUTHED && AuthUtils::authState != AuthUtils::ERROR) return;
        SetPrompt("<color=green>Successfully Authenticated!</color>", 5);
        prompt_loader->set_active(false);
        playerUsername->SetText(localPlayerInfo.username);
        BSML::Utilities::SetImage(playerAvatar, string_format("%suser/%s/avatar", Constants::BASE_URL.c_str(), localPlayerInfo.userID.c_str()));
        playerAvatar->set_material(GetRoundEdgeMaterial());
        playerAvatarLoading->SetActive(false);
        Lapiz::Utilities::MainThreadScheduler::Schedule([this](){
            seasonText->set_richText(true);
            SetSeasons(15);
        });
        leaderboard.get_bedroomPartyStaffAsync([this](std::vector<std::string> staff){
            if (std::find(staff.begin(), staff.end(), localPlayerInfo.userID) != staff.end()){
                playerUsername->get_gameObject()->AddComponent<RainbowAnimation*>();
            }
        });
    }

    void PanelViewController::SetPrompt(StringW text, int time){
        if (!prompt_text) return;
        prompt_text->SetText(text);
        TweeningUtils::FadeText(prompt_text, true, 0.2f);
        text = prompt_text->get_text();
        if (time < 0) return;
        std::thread([this, text, time](){
            std::this_thread::sleep_for(std::chrono::seconds(time));
            Lapiz::Utilities::MainThreadScheduler::Schedule([this, text](){
                if (prompt_text->get_text() != text) return;
                TweeningUtils::FadeText(prompt_text, false, 0.15f);
            });
        }).detach();
    }
}