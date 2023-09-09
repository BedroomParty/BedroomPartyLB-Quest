#pragma once

#include "rapidjson-macros/shared/macros.hpp"
#include <string>

namespace BedroomPartyLB::Models{
    
    DECLARE_JSON_CLASS(RequestBody,
        NAMED_VALUE(int, difficulty, "difficulty");
        NAMED_VALUE(std::string, characteristic, "characteristic");
        NAMED_VALUE(std::string, userID, "id");
        NAMED_VALUE(int, multipliedScore, "multipliedScore");
        NAMED_VALUE(int, modifiedScore, "modifiedScore");
        NAMED_VALUE(float, accuracy, "accuracy");
        NAMED_VALUE(int, misses, "misses");
        NAMED_VALUE(int, badCuts, "badCuts");
        NAMED_VALUE(bool, fullCombo, "fullCombo");
        NAMED_VALUE(std::string, modifiers, "modifiers");

        RequestBody() = default;
        RequestBody(int diff, std::string charac, std::string id, int multipliedScore, int modifiedScore,
                    float acc, int misses, int badcuts, bool fc, std::string modifiers) :
                    difficulty(diff), characteristic(charac), userID(id), multipliedScore(multipliedScore), modifiedScore(modifiedScore), 
                    accuracy(acc), misses(misses), badCuts(badcuts), fullCombo(fc), modifiers(modifiers){}    
    )
}
