#pragma once

#include "rapidjson-macros/shared/macros.hpp"
#include <string>

namespace BedroomPartyLB::Models
{
    DECLARE_JSON_CLASS(ScoreUploadBody,
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
        NAMED_VALUE(int, pauses, "pauses");
        NAMED_VALUE(float, accLeft, "leftHandAccuracy");
        NAMED_VALUE(float, accRight, "rightHandAccuracy");
        NAMED_VALUE(float, tdLeft, "leftHandTimeDependency");
        NAMED_VALUE(float, tdRight, "rightHandTimeDependency");
        NAMED_VALUE(int, perfStreak, "perfectStreak");
        NAMED_VALUE(float, fcAccuracy, "fcAccuracy");

        ScoreUploadBody() = default;
        ScoreUploadBody(int diff, std::string charac, std::string id, int multipliedScore, int modifiedScore,
                    float acc, int misses, int badcuts, bool fc, std::string modifiers, int pause, 
                    float accL, float accR, float tdL, float tdR, int streak, float fcAccuracy) :
                    difficulty(diff), characteristic(charac), userID(id), multipliedScore(multipliedScore), modifiedScore(modifiedScore), 
                    accuracy(acc), misses(misses), badCuts(badcuts), fullCombo(fc), modifiers(modifiers), 
                    pauses(pause), accLeft(accL), accRight(accR), tdLeft(tdL), tdRight(tdR), perfStreak(streak), fcAccuracy(fcAccuracy){}  
        public:
        std::string toString() const
        {
            return WriteToString(*this);
        }  
    )
}
