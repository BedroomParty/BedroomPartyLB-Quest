#include "PluginUI/LeaderboardViewController.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IDifficultyBeatmapSet.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IBeatmapLevelData.hpp"
#include "GlobalNamespace/LeaderboardTableView.hpp"
#include "System/Collections/Generic/List_1.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/rapidjson.h"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "bsml/shared/BSML.hpp"
#include "assets.hpp"
#include "WebUtils.hpp"
#include "GlobalNamespace/LeaderboardTableView_ScoreData.hpp"
#include "GlobalNamespace/LeaderboardTableCell.hpp"
#include "logging.hpp"
#include "main.hpp"
#include <iomanip>
#include <ios>
#include <sstream>
#include "bsml/shared/Helpers/utilities.hpp"
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"
using ScoreData = GlobalNamespace::LeaderboardTableView::ScoreData;
using List_1 = System::Collections::Generic::List_1<ScoreData>;

DEFINE_TYPE(BedroomPartyLB::UI, LeaderboardViewController);

namespace BedroomPartyLB::UI
{
    int page = 0;
    void LeaderboardViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (!firstActivation) return;
        BSML::parse_and_construct(IncludedAssets::LeaderboardView_bsml, this->get_transform(), this);
    }

    void LeaderboardViewController::PostParse()
    {

    }

    void RichMyText(GlobalNamespace::LeaderboardTableView *tableView)
    {
        for (auto &cell : tableView->GetComponentsInChildren<GlobalNamespace::LeaderboardTableCell*>())
        {
            cell->playerNameText->set_richText(true);
        }
    }

    void LeaderboardViewController::RefreshLeaderboard(GlobalNamespace::IDifficultyBeatmap *difficultyBeatmap) 
    {
        Lapiz::Utilities::MainThreadScheduler::Schedule([&]() {
            std::string beatmapID = difficultyBeatmap->get_level()->i_IPreviewBeatmapLevel()->get_levelID()->Substring(13);
            std::string difficulty = std::to_string(difficultyBeatmap->get_difficultyRank());
            std::string characteristic = difficultyBeatmap->get_parentDifficultyBeatmapSet()->get_beatmapCharacteristic()->serializedName;

            std::string url = BASE_URL + "leaderboard/"+beatmapID+"?char="+characteristic+"&diff="+difficulty+"&sort=top&limit=10&page="+std::to_string(page)+"&id="+userID.c_str();
           
            WebUtils::GetAsync(url, [this](std::string value, bool success) {
                if (success)
                {
                    rapidjson::Document doc;
                    doc.Parse(value.c_str());
                    if (doc.HasMember("scores"))
                    {
                        auto scoreArray = doc["scores"].GetArray();

                        List<ScoreData*> *scores = List<ScoreData*>::New_ctor();
                        for (int i = 0; i < scoreArray.Size(); i++)
                        {
                            int score = scoreArray[i]["modifiedScore"].GetInt();
                            std::string username = "<color=blue>" + std::string(scoreArray[i]["username"].GetString()) + "</color>";
                            std::stringstream ss;
                            ss << std::fixed << std::setprecision(2) << scoreArray[i]["accuracy"].GetFloat();
                            std::string accuracy = " - (<color=#ffd42a>" + ss.str() + "%</color>)";
                            std::string misses = std::to_string((scoreArray[i]["misses"].GetInt() + scoreArray[i]["badCuts"].GetInt()));
                            std::string combo = scoreArray[i]["fullCombo"].GetBool() ? "<color=green> FC </color>" : " - <color=red>x" + misses + "</color>" ;
                            std::string modifiers = "<size=60%>" + std::string(scoreArray[i]["modifiers"].GetString()) + "</size>";
                            
                            std::string result = "<size=90%>" + username + accuracy + combo + modifiers + "</size>";
                            ScoreData *scoreData = ScoreData::New_ctor(score, result, i + 1, false);
                            scores->Add(scoreData);
                        }
                        if (scores->size > 0)
                        {
                            errorText->get_gameObject()->SetActive(false);
                            BPLeaderboard->SetScores(scores, -1);
                            RichMyText(BPLeaderboard);
                        }
                        else 
                        {
                            errorText->get_gameObject()->SetActive(true);
                            BPLeaderboard->SetScores(nullptr, -1);
                        }
                    }

                    //BPLeaderboard->SetScores(scores, -1)
                }
            });
        });
        
    }
}