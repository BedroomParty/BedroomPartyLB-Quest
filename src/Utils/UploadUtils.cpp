#include "Utils/UploadUtils.hpp"
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"
#include "Models/LocalPlayerInfo.hpp"
#include "Models/CustomLeaderboard.hpp"
#include "Utils/WebUtils.hpp"
#include "Utils/AwaitUtils.hpp"
#include "Utils/AuthUtils.hpp"

namespace BedroomPartyLB::UploadUtils
{
    using namespace System::Net;
    using namespace AwaitUtils;
    using namespace System::Threading;

    void HandleScoreUploadResult(bool success, std::string errorMessage)
    {
        Lapiz::Utilities::MainThreadScheduler::Schedule([success, errorMessage]()
        {
            if (!success) return leaderboard.get_panelViewController()->SetPrompt(errorMessage, 7);
            leaderboard.get_panelViewController()->SetPrompt(errorMessage, 5);
            leaderboard.get_leaderboardViewController()->RefreshLeaderboard(leaderboard.currentDifficultyBeatmap); 
        });
    }

    std::string RetrieveErrorMessage(int responseCode)
    {
        if (responseCode == HttpStatusCode::OK) return "<color=#43e03a>Successfully uploaded score!</color>";
        if (responseCode == HttpStatusCode::Conflict) return "<color=#f0584a>Failed to upload. Did not beat previous score</color>";
        return "<color=#f0584a>Failed to upload...</color>";
    }

    void TryUploadScore(std::string url, std::string body)
    {
        if (AuthUtils::authState == AuthUtils::ERROR) return HandleScoreUploadResult(false, "<color=#f0584a>Failed to upload...</color>");

        long time = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        if (time > localPlayerInfo.sessionKeyExpires)
        {
            bool success = false;
            for (int i = 0; i < 3; i++)
            {
                success = AwaitValue(&AuthUtils::RequestNewSessionKey);
                if (!success) Thread::Sleep(2000);
                else break;
            }
            if (!success) return HandleScoreUploadResult(false, "<color=#f0584a>Failed to upload...</color>");
        }
        bool uploadSuccess = false;
        int responseCode;
        for (int i = 0; i < 3; i++)
        {
            std::tie(uploadSuccess, responseCode) = AwaitValue(&UploadScore, url, body);
            if (!uploadSuccess && responseCode != HttpStatusCode::Conflict) Thread::Sleep(2000);
            else break;
        }
        return HandleScoreUploadResult(uploadSuccess, RetrieveErrorMessage(responseCode));
    }

    void UploadScore(std::string url, std::string requestBody, std::function<void(bool,int)> callback)
    {
        BedroomPartyLB::WebUtils::PostAsync(url, requestBody, false, [callback](std::string value, bool success, int responseCode)
        {
            callback(success, responseCode); 
        });
    }
}