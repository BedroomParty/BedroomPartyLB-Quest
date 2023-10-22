#pragma once

#include <vector>
#include "main.hpp"

namespace BedroomPartyLB::Models{

    typedef struct ExtraSongData{
        int pauses = 0;
        std::vector<int> rightHandAverageScore;
        std::vector<int> leftHandAverageScore;
        std::vector<float> rightHandTimeDependency;
        std::vector<float> leftHandTimeDependency;
        int maxPossibleScore = 0;
        int perfectStreak = 0;

        inline void reset() {
            pauses = 0;
            rightHandAverageScore.clear();
            leftHandAverageScore.clear();
            rightHandTimeDependency.clear();
            leftHandTimeDependency.clear();
            maxPossibleScore = 0;
            perfectStreak = 0;
        }

        int GetTotalFromList(const std::vector<int>& list) const
        {
            if (list.empty()) return 0;
            int sum = 0;
            for (const auto& f : list) sum += f;
            return sum;
        }

        template<typename T>
        float GetAverageFromList(const std::vector<T>& list) const
        {
            if (list.empty()) return 0;
            T sum = 0;
            for (const T& f : list) sum += f;
            return sum / (float)list.size();
        }

        inline float GetFcAcc(float multiplier) const
        {
            if (maxPossibleScore == 0) return 0.0f;
            int realScore = static_cast<int>(std::floor((float)(GetTotalFromList(leftHandAverageScore) + GetTotalFromList(rightHandAverageScore)) * multiplier));
            float fcAcc = (float)realScore/(float)maxPossibleScore * 100.0f;
            return fcAcc;
        }
    }ExtraSongData;
}
extern BedroomPartyLB::Models::ExtraSongData extraSongData;