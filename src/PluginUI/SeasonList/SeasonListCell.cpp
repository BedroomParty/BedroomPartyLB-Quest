#include "PluginUI/SeasonList/SeasonListCell.hpp"

#include "HMUI/Touchable.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/Networking/DownloadHandlerTexture.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/UnityWebRequestTexture.hpp"
#include "Utils/WebUtils.hpp"

#include "assets.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/Helpers/getters.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
#include "Utils/StringUtils.hpp"
#include "UnityEngine/Application.hpp"
#include "Models/CustomLeaderboard.hpp"

DEFINE_TYPE(BedroomPartyLB::UI, SeasonListCell);

using namespace UnityEngine;
using namespace UnityEngine::UI;

UnityEngine::Color highlightedColor = UnityEngine::Color(0.0f, 0.71f, 0.82f, 0.8f);
UnityEngine::Color idleColor = UnityEngine::Color(0.0f, 0.0f, 0.0f, 0.7f);

namespace BedroomPartyLB::UI
{
    SeasonListCell* SeasonListCell::CreateNewCell()
    {
        static ConstString playerTableCellStr("SeasonListCell");
        auto cellGO = UnityEngine::GameObject::New_ctor();
        auto playerCell = cellGO->AddComponent<SeasonListCell*>();
        cellGO->set_name(playerTableCellStr);

        BSML::parse_and_construct(IncludedAssets::SeasonListCell_bsml, cellGO->get_transform(), playerCell);
        return playerCell;
    }

    void SeasonListCell::PostParse()
    {
        get_gameObject()->AddComponent<HMUI::Touchable*>();
        backgroundImage->background->set_color0(idleColor);
        backgroundImage->background->set_color1(idleColor);
        seasonImage->set_material(leaderboard.get_panelViewController()->GetRoundEdgeMaterial());
    }

    void SeasonListCell::Populate(Models::SeasonData &data)
    {
        _data = &data;
        set_seasonNumber(_data->seasonNumber);
        set_seasonDescription(_data->seasonDescription);
        set_seasonRank(_data->seasonRank);
        set_seasonPP(_data->seasonPP);

        if (_data->seasonImageSprite != nullptr && _data->seasonImageSprite->m_CachedPtr.m_value != nullptr && _data->seasonImageSprite->get_texture() != nullptr && _data->seasonImageSprite->get_texture()->m_CachedPtr.m_value != nullptr)
            return set_sprite(_data->seasonImageSprite);

        if (_data->seasonImageLink.empty())
            return;
        downloadInProgress = true;
        WebUtils::GetImageAsync(_data->seasonImageLink, [this](UnityEngine::Sprite* profilePicture)
        {
            _data->seasonImageSprite = profilePicture;
            set_sprite(profilePicture);
            downloadInProgress = false; 
        });
    }

    void SeasonListCell::SeasonLeaderboardClicked()
    {
        UnityEngine::Application::OpenURL("https://thebedroom.party/season/" + std::to_string(_data->seasonNumber));
    }

    void SeasonListCell::SeasonPlaylistClicked()
    {
        UnityEngine::Application::OpenURL("https://thebedroom.party/playlist/" + std::to_string(_data->seasonNumber));
    }

    void SeasonListCell::HighlightDidChange(HMUI::SelectableCell::TransitionType transitionType)
    {
        backgroundImage->background->set_color1(get_highlighted() ? highlightedColor : idleColor);
        backgroundImage->background->set_color(get_highlighted() ? highlightedColor : idleColor);
    }

    void SeasonListCell::FixedUpdate()
    {
        if (!get_gameObject()->get_activeInHierarchy() || downloadInProgress)
            return;

        if (_data->seasonImageSprite != nullptr && _data->seasonImageSprite->m_CachedPtr.m_value != nullptr && _data->seasonImageSprite->get_texture() != nullptr && _data->seasonImageSprite->get_texture()->m_CachedPtr.m_value != nullptr)
            return;
        if (_data->seasonImageLink.empty())
            return;
        downloadInProgress = true;
        WebUtils::GetImageAsync(_data->seasonImageLink, [this](UnityEngine::Sprite* seasonImageSprite)
        {
            _data->seasonImageSprite = seasonImageSprite;
            set_sprite(seasonImageSprite);
            downloadInProgress = false; 
        });
    }

    void SeasonListCell::set_seasonNumber(int num)
    {
        seasonNumberText->SetText(string_format("Season: %i", num));
    }

    void SeasonListCell::set_seasonDescription(std::string_view desc)
    {
        seasonDescriptionText->SetText(desc);
    }

    void SeasonListCell::set_seasonRank(int rank)
    {
        rankText->SetText(string_format("Rank: %i", rank));
    }

    void SeasonListCell::set_seasonPP(float pp)
    {
        ppText->SetText(string_format("PP: %i", (int)pp));
    }

    void SeasonListCell::set_sprite(UnityEngine::Sprite *sprite)
    {
        seasonImage->set_sprite(sprite);
    }
}