#pragma once

#include <vector>
#include "main.hpp"

namespace BedroomPartyLB::Models{

    typedef struct ExtraSongData{
        int pauses = 0;
        std::vector<int> avgHandAccRight;
        std::vector<int> avgHandAccLeft;
        std::vector<float> avgHandTDRight;
        std::vector<float> avgHandTDLeft;
        std::vector<std::pair<int,int>> totalBlocksHit;
        int perfectStreak = 0;

        inline void reset() {
            pauses = 0;
            avgHandAccRight.clear();
            avgHandAccLeft.clear();
            avgHandTDRight.clear();
            avgHandTDLeft.clear();
            totalBlocksHit.clear();
            perfectStreak = 0;
        }

        template<typename T>
        float GetAverageFromList(const std::vector<T>& list) const
        {
            if (list.empty()) return 0;
            T sum = 0;
            for (const T& f : list) sum += f;
            return sum / (float)list.size();
        }

        int GetMaxScoreForScoringType(int scoringType) const
        {
            switch (scoringType)
            {
                case 1:
                case 2:
                case 3: 
                    return 115;
                case 4:
                    return 70;
                case 5:
                    return 20;
                default:
                    return 0;
            }
        }

        inline float GetFcAcc() const
        {
            if (totalBlocksHit.empty()) return 0.0f;
            int realScore = 0, maxScore = 0;
            for (auto& p : totalBlocksHit)
            {
                realScore += p.first;
                maxScore += GetMaxScoreForScoringType(p.second);
            }
            float fcAcc = (float)realScore/(float)maxScore * 100.0f;
            return fcAcc;
        }

    }ExtraSongData;
}
extern BedroomPartyLB::Models::ExtraSongData extraSongData;