#pragma once

#include "rapidjson-macros/shared/macros.hpp"
#include <string>
#include "GlobalNamespace/LeaderboardTableView_ScoreData.hpp"
#include "beatsaber-hook/shared/utils/utils-functions.h"
#include "Utils/StringUtils.hpp"

namespace BedroomPartyLB::Models
{

    class FlexibleFloat : public TypeOptions<int, float, double> {
        public: operator float() const {
            if(auto v = GetValue<float>())
                return *v;
            else if (auto v = GetValue<double>())
                return *v;
            return *GetValue<int>();
        }
        using TypeOptions<int, float, double>::TypeOptions;
    };
    
    DECLARE_JSON_CLASS(BPLeaderboardEntry,
        NAMED_VALUE_OPTIONAL(std::string, userID, "id");
        NAMED_VALUE_DEFAULT(int, modifiedScore, 0, "modifiedScore");
        NAMED_VALUE_DEFAULT(int, multipliedScore, 0, "multipliedScore");
        NAMED_VALUE_DEFAULT(FlexibleFloat, accuracy, 0.0f, "accuracy");
        NAMED_VALUE_DEFAULT(int, misses, -1, "misses");
        NAMED_VALUE_DEFAULT(int, badCuts, -1, "badCuts");
        NAMED_VALUE_DEFAULT(bool, fullCombo, false, "fullCombo");
        NAMED_VALUE_DEFAULT(std::string, modifiers, "", "modifiers");
        NAMED_VALUE_DEFAULT(std::string, username, "", "username");
        NAMED_VALUE_DEFAULT(long, timeSet, 0, "timeSet");
        NAMED_VALUE_DEFAULT(int, rank, 0, "position");
        NAMED_VALUE_DEFAULT(int, pauses, 0, "pauses");
        NAMED_VALUE_DEFAULT(FlexibleFloat, accL, 0, "avgHandAccRight");
        NAMED_VALUE_DEFAULT(FlexibleFloat, accR, 0, "avgHandAccLeft");
        NAMED_VALUE_DEFAULT(FlexibleFloat, tdL, 0, "avgHandTDRight");
        NAMED_VALUE_DEFAULT(FlexibleFloat, tdR, 0, "avgHandTDLeft");
        NAMED_VALUE_DEFAULT(int, perfectStreak, 0, "perfectSteak");

        // DESERIALIZE_ACTION(0,
        //     self->accuracy = float(self->acc);
        // )

        public:
        // float accuracy = 0.0f;
        
        GlobalNamespace::LeaderboardTableView::ScoreData* CreateLeaderboardEntryData(int rank) const 
        {

            std::string name = this->username;
            std::string acc = string_format(" - (<color=#ffd42a>%s%%</color>)", StringUtils::format_float(this->accuracy).c_str());

            std::string missCount = std::to_string(this->misses + this->badCuts);
            std::string combo = this->fullCombo ? "<color=green> FC </color>" : " - <color=red>x" + missCount + "</color>" ;
            std::string mods = " <size=60%>" + modifiers + "</size>";
            
            if (!this->userID.has_value() || username == "") name = "<color=red>Error</color>";
            else if (this->userID.value() == "76561199077754911") name = "<color=#6488ea><rotate=180>" + std::string(name.rbegin(), name.rend()) + "</rotate></color>";

            std::string result = "<size=90%>" + name + acc + combo + mods + "</size>";
            return GlobalNamespace::LeaderboardTableView::ScoreData::New_ctor(this->modifiedScore, result, rank, false);
        }
    )

    DECLARE_JSON_CLASS(BPLeaderboard,
        NAMED_VALUE(int, scoreCount, "scoreCount");
        NAMED_VECTOR(BPLeaderboardEntry, scores, "scores");
    )
}