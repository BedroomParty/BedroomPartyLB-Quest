#pragma once

#include <vector>

namespace BedroomPartyLB::Models{

    typedef struct ExtraSongData{
        int pauses = 0;
        std::vector<float> avgHandAccRight;
        std::vector<float> avgHandAccLeft;
        std::vector<float> avgHandTDRight;
        std::vector<float> avgHandTDLeft;
        int perfectStreak = 0;

        inline void reset() {
            pauses = 0;
            avgHandAccRight.clear();
            avgHandAccLeft.clear();
            avgHandTDRight.clear();
            avgHandTDLeft.clear();
            perfectStreak = 0;
        }

        float GetAverageFromList(std::vector<float> list) const{
            if (list.size() == 0) return 0;
            float sum = 0;
            for (const float& f : list) sum += f;
            return sum / (float)list.size();
        }
    }ExtraSongData;
}
extern BedroomPartyLB::Models::ExtraSongData extraSongData;