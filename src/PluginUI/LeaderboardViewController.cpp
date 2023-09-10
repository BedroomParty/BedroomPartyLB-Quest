#include "PluginUI/LeaderboardViewController.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IDifficultyBeatmapSet.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IBeatmapLevelData.hpp"
#include "GlobalNamespace/LeaderboardTableView.hpp"
#include "System/Collections/Generic/List_1.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "bsml/shared/BSML.hpp"
#include "assets.hpp"
#include "GlobalNamespace/LeaderboardTableView_ScoreData.hpp"
#include "GlobalNamespace/LeaderboardTableCell.hpp"
#include "logging.hpp"
#include "main.hpp"
#include <iomanip>
#include <ios>
#include <sstream>
#include "bsml/shared/Helpers/utilities.hpp"
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"
#include "Downloaders/LeaderboardDownloader.hpp"
#include "Models/CustomLeaderboard.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/Constants.hpp"
#include "bsml/shared/Helpers/getters.hpp"
#include "bsml/shared/Helpers/utilities.hpp"
#include "System/Guid.hpp"
#include "Utils/AuthUtils.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "Utils/ArrayUtil.hpp"
#include "Utils/WebUtils.hpp"
#include "PluginUI/ScoreInfoModal.hpp"
#include "PluginUI/CellClicker.hpp"
#include "Models/LocalPlayerInfo.hpp"

using ScoreData = GlobalNamespace::LeaderboardTableView::ScoreData;
using List_1 = System::Collections::Generic::List_1<ScoreData>;
using namespace HMUI;
using namespace UnityEngine::UI;

DEFINE_TYPE(BedroomPartyLB::UI, LeaderboardViewController);

extern BedroomPartyLB::Models::CustomLeaderboard leaderboard;

namespace BedroomPartyLB::UI
{
    int scope = 0;
    std::string currentRefreshId;

    void LeaderboardViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation) {
            scoreInfoModal = ScoreInfoModal::New_ctor();
            BSML::parse_and_construct(IncludedAssets::LeaderboardView_bsml, this->get_transform(), this);
            GetComponentsInChildren<VerticalLayoutGroup*>().First([](auto& v){return v->get_spacing()==-19.4f;})
                ->GetComponentsInChildren<ImageView*>()->copy_to(playerAvatars);
            ArrayUtil::Where(GetComponentsInChildren<VerticalLayoutGroup*>().First([](auto& v){return v->get_spacing()==-19.4f;})
                ->GetComponentsInChildren<ImageView*>(true), [](ImageView* img){return img != nullptr && !img->get_gameObject()->get_active();})->copy_to(avatarLoadings);
            for (auto img : playerAvatars) img->set_sprite(nullptr);
        }
        CheckPage();
        RefreshLeaderboard(leaderboard.currentDifficultyBeatmap);
    }

    void LeaderboardViewController::PostParse()
    {
        myHeader->background->set_material(BSML::Helpers::GetUINoGlowMat());
        myHeader->background->skew = 0.18f;
        myHeader->background->gradient = true;
        myHeader->background->set_color(Constants::BP_COLOR);
        myHeader->background->set_color0(Constants::BP_COLOR);
        myHeader->background->set_color1(Constants::BP_COLOR);

        scopeControl->SetData(ArrayW<IconSegmentedControl::DataItem*>({
            IconSegmentedControl::DataItem::New_ctor(BSML::Utilities::LoadSpriteRaw(IncludedAssets::Globe_png), "Bedroom Party"),
            IconSegmentedControl::DataItem::New_ctor(BSML::Utilities::LoadSpriteRaw(IncludedAssets::Player_png), "Around You")
        }));
    }

    void LeaderboardViewController::CheckPage()
    {
        if (!this->isActivated) return;
        up_button->set_interactable(page > 0);
        down_button->set_interactable(scope == 0);
    }

    void LeaderboardViewController::ChangeScope()
    {
        page = 0;
        RefreshLeaderboard(leaderboard.currentDifficultyBeatmap);
        CheckPage();
    }

    void LeaderboardViewController::OnIconSelected(IconSegmentedControl* segmentedControl, int index)
    {
        scope = index;
        ChangeScope();
    }

    void LeaderboardViewController::OnPageUp()
    {
        page--;
        RefreshLeaderboard(leaderboard.currentDifficultyBeatmap);
        CheckPage();
    }

    void LeaderboardViewController::OnPageDown()
    {
        page++;
        RefreshLeaderboard(leaderboard.currentDifficultyBeatmap);
        CheckPage();
    }

    void LeaderboardViewController::SetLoading(bool value, std::string error){
        leaderboard_loading->set_active(value);
        errorText->get_gameObject()->set_active(!value && error != "");
        if (error == "") return;
        errorText->SetText("blah");
        errorText->SetText(error);
    }

    void LeaderboardViewController::RichMyText(GlobalNamespace::LeaderboardTableView *tableView, std::vector<Models::BPLeaderboardEntry> entries)
    {
        for (auto &cell : tableView->GetComponentsInChildren<GlobalNamespace::LeaderboardTableCell*>())
        {
            cell->set_showSeparator(true);
            cell->playerNameText->set_richText(true);
            cell->rankText->set_richText(true);
            cell->scoreText->set_richText(true);
            cell->rankText->SetText(string_format("<size=120%%><u>%s</u></size>", std::string(cell->rankText->get_text()).c_str()));
            CellClicker* clicker = cell->get_gameObject()->GetComponent<CellClicker*>();
            if (!clicker) clicker = cell->get_gameObject()->AddComponent<CellClicker*>();

            clicker->onClick = std::bind(&ScoreInfoModal::setScoreModalText, this->scoreInfoModal, entries[cell->idx], cell->idx);
            clicker->separator = reinterpret_cast<HMUI::ImageView*>(cell->separatorImage);
            clicker->separator->set_color(Constants::BP_COLOR2);
            clicker->separator->set_color0(UnityEngine::Color::get_white());
            clicker->separator->set_color1(UnityEngine::Color(1, 1, 1, 0));
        }
    }

    List<ScoreData*>* LeaderboardViewController::CreateLeaderboardData(std::vector<Models::BPLeaderboardEntry> leaderboard){
        auto tableData = List<ScoreData*>::New_ctor();
        int rank = 0;
        for (auto& entry : leaderboard) tableData->Add(entry.CreateLeaderboardEntryData(((page + 1) * 10) - (10 - (++rank))));
        return tableData;
    }

    int GetPlayerScoreIndex(std::vector<Models::BPLeaderboardEntry> scores)
    {
        for (int i = 0; i < scores.size(); i++)
        {
            if (scores[i].userID.value_or("knob") == localPlayerInfo.userID) return i;
        }
        return -1;
    }

    void LeaderboardViewController::RefreshLeaderboard(GlobalNamespace::IDifficultyBeatmap *difficultyBeatmap) 
    {
        if (!this->isActivated) return;
        if (difficultyBeatmap == nullptr) return;
        if (AuthUtils::authState == AuthUtils::ERROR) return SetLoading(false, "Auth Failed");
        if (AuthUtils::authState != AuthUtils::AUTHED) return SetLoading(false, "Authenticating...");

        std::string refreshId = std::string(System::Guid::NewGuid().ToString());
        currentRefreshId = refreshId;
        BPLeaderboard->tableView->SetDataSource(nullptr, true);
        for (auto image : playerAvatars) image->get_gameObject()->set_active(false);
        for (auto image : avatarLoadings) image->get_gameObject()->set_active(false);
        AnnihilatePlayerSprites();
        SetLoading(true);
        std::thread t([difficultyBeatmap, refreshId, this] {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (currentRefreshId != refreshId) return;
            Lapiz::Utilities::MainThreadScheduler::Schedule([difficultyBeatmap, refreshId, this]() {
                Downloaders::DownloadLeaderboardAsync(difficultyBeatmap, page, scope, [this, refreshId](std::optional<Models::BPLeaderboard> pageLeaderboard){
                    if (currentRefreshId != refreshId) return;
                    if (pageLeaderboard.has_value() && pageLeaderboard.value().scores.size() > 0){
                        std::vector<Models::BPLeaderboardEntry> scores = pageLeaderboard.value().scores;
                        auto scoreData = CreateLeaderboardData(scores);
                        errorText->get_gameObject()->SetActive(false);
                        BPLeaderboard->SetScores(scoreData, GetPlayerScoreIndex(scores));
                        RichMyText(BPLeaderboard, pageLeaderboard.value().scores);
                        SetLoading(false);
                        SetPlayerSprites(scores, refreshId);
                        return;
                    }
                    std::string failedText = scope == 0 ? page > 0 ? "No scores on this page!" : "No scores on this map!" : "Set a score on this map!";
                    SetLoading(false, failedText);
                });
            });
        });
        t.detach();
    }

    void LeaderboardViewController::SetPlayerSprites(std::vector<BedroomPartyLB::Models::BPLeaderboardEntry> players, std::string refreshId){
        for (auto image : playerAvatars) image->get_gameObject()->set_active(false);
        for (int i=0; i<players.size(); i++){
            if (players[i].userID.has_value()){
                avatarLoadings[i]->get_gameObject()->set_active(true);
                std::string url = string_format("https://api.thebedroom.party/user/%s/avatar", players[i].userID.value().c_str());
                WebUtils::GetImageAsync(url, [i, refreshId, this](UnityEngine::Sprite* sprite){
                    Lapiz::Utilities::MainThreadScheduler::Schedule([i, refreshId, sprite, this](){
                        if (currentRefreshId != refreshId){
                            if (sprite) Object::Destroy(sprite->get_texture());
                            Object::Destroy(sprite);
                            avatarLoadings[i]->get_gameObject()->set_active(false);
                            return;
                        }
                        playerAvatars[i]->set_sprite(sprite);
                        playerAvatars[i]->get_gameObject()->set_active(true);
                        avatarLoadings[i]->get_gameObject()->set_active(false);
                    });
                });
            }
            else avatarLoadings[i]->get_gameObject()->set_active(false);
        }
    }

    void LeaderboardViewController::AnnihilatePlayerSprites(){
        for (auto image : playerAvatars){
            if (image->get_sprite() && image->get_sprite()->m_CachedPtr.m_value) Object::Destroy(image->get_sprite()->get_texture());
            Object::Destroy(image->get_sprite());
            image->get_gameObject()->set_active(false);
        }
    }
}