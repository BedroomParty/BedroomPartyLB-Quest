#pragma once

#include <vector>

namespace BedroomPartyLB::Models{

    typedef struct ExtraSongData{
        int pauses = 0;
        std::vector<int> avgHandAccRight;
        std::vector<int> avgHandAccLeft;
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

        int GetTotalFromList(std::vector<int> list) const
        {
            int result = 0;
            for (const auto& value : list) result += value;
            return result;
        }

        template<typename T>
        float GetAverageFromList(std::vector<T> list) const
        {
            if (list.empty()) return 0;
            T sum = 0;
            for (const T& f : list) sum += f;
            return sum / (float)list.size();
        }

        float GetFcAcc() const
        {
            int blocksHit = avgHandAccLeft.size() + avgHandAccRight.size();
            if (blocksHit == 0) return 0.0f;
            float averageHitScore = (float)(GetTotalFromList(avgHandAccLeft) + GetTotalFromList(avgHandAccRight)) / (float)blocksHit;
            float fcAcc = averageHitScore / 115.0f * 100.0f;
            return fcAcc;
        }

    }ExtraSongData;
}
extern BedroomPartyLB::Models::ExtraSongData extraSongData;