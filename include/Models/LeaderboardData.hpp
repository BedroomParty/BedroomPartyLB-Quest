#pragma once

#include "rapidjson-macros/shared/macros.hpp"
#include <string>
#include "GlobalNamespace/LeaderboardTableView_ScoreData.hpp"
#include "beatsaber-hook/shared/utils/utils-functions.h"

namespace BedroomPartyLB::Models{
    
    DECLARE_JSON_CLASS(BPLeaderboardEntry,
        NAMED_VALUE_OPTIONAL(std::string, userID, "id");
        NAMED_VALUE(int, modifiedScore, "modifiedScore");
        NAMED_VALUE(int, multipliedScore, "multipliedScore");
        NAMED_VALUE(float, accuracy, "accuracy");
        NAMED_VALUE(int, misses, "misses");
        NAMED_VALUE(int, badCuts, "badCuts");
        NAMED_VALUE(bool, fullCombo, "fullCombo");
        NAMED_VALUE(std::string, modifiers, "modifiers");
        NAMED_VALUE_DEFAULT(std::string, username, "null", "username");

        public:
        GlobalNamespace::LeaderboardTableView::ScoreData* CreateLeaderboardEntryData(int rank) const {

            std::string name = this->username;
            std::string acc = string_format(" - (<color=#ffd42a>%.2f%%</color>)", this->accuracy);
            std::string missCount = std::to_string(this->misses + this->badCuts);
            std::string combo = this->fullCombo ? "<color=green> FC </color>" : " - <color=red>x" + missCount + "</color>" ;
            std::string mods = "<size=60%>" + modifiers + "</size>";
            
            std::string result = "<size=90%>" + name + acc + combo + mods + "</size>";

            if (this->userID.has_value() && this->userID.value() == "76561199077754911")
            {
                name = "<color=blue>" + this->username + "</color>";
            }
            return GlobalNamespace::LeaderboardTableView::ScoreData::New_ctor(this->modifiedScore, result, rank, false);
        }
    )

    DECLARE_JSON_CLASS(BPLeaderboard,
        NAMED_VALUE(int, scoreCount, "scoreCount");
        NAMED_VECTOR(BPLeaderboardEntry, scores, "scores");
    )
}