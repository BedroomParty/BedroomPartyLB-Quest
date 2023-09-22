#include "Utils/UploadUtils.hpp"
#include <thread>
#include <future>
#include "Utils/AuthUtils.hpp"
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"
#include "Models/LocalPlayerInfo.hpp"
#include "Models/CustomLeaderboard.hpp"
#include "Utils/WebUtils.hpp"

template <typename T>
using AwaitableFunc = std::function<void(std::function<void(T)>)>;

namespace BedroomPartyLB::UploadUtils
{
    using namespace System::Net;

    int tryAgain = true;

    auto getSessionKey(std::function<void(bool)> callback)
    {
        AuthUtils::RequestNewSessionKey([callback](bool success)
        { 
            callback(success); 
        });
    }

    template <typename T>
    T AwaitValue(AwaitableFunc<T> func)
    {
        T *output = new T();
        bool *inProgress = new bool(true);
        Lapiz::Utilities::MainThreadScheduler::Schedule([func, output, inProgress]()
        { 
            func([output, inProgress](T value)
            {
                *output = value;
                *inProgress = false; 
            }); 
        });
        while (*inProgress) std::this_thread::yield();
        T newval = T(*output);
        delete output;
        delete inProgress;
        return newval;
    }

    void HandleScoreUploadResult(bool success)
    {
        Lapiz::Utilities::MainThreadScheduler::Schedule([success]()
        {
            if (!success) return leaderboard.get_panelViewController()->SetPrompt("<color=#f0584a>Failed to upload...</color>", 7);
            leaderboard.get_panelViewController()->SetPrompt("<color=#43e03a>Successfully uploaded score!</color>", 5);
            leaderboard.get_leaderboardViewController()->RefreshLeaderboard(leaderboard.currentDifficultyBeatmap); 
        });
    }

    void TryUploadScore(std::string url, std::string body)
    {
        if (AuthUtils::authState == AuthUtils::ERROR) return HandleScoreUploadResult(false);
        leaderboard.get_panelViewController()->SetPrompt("Uploading Score...", -1, true);

        long time = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        if (time > localPlayerInfo.sessionKeyExpires)
        {
            bool success = false;
            for (int i = 0; i < 3; i++)
            {
                success = AwaitValue(std::function(&getSessionKey));
                if (!success) std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                else break;
            }
            if (!success) return HandleScoreUploadResult(false);
        }
        tryAgain = true;
        bool uploadSuccess = false;
        AwaitableFunc<bool> upload = std::bind(&UploadScore, url, body, std::placeholders::_1);
        for (int i = 0; i < 3; i++)
        {
            uploadSuccess = AwaitValue(upload);
            if (!uploadSuccess && tryAgain) std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            else break;
        }
        return HandleScoreUploadResult(uploadSuccess);
    }

    void UploadScore(std::string url, std::string requestBody, std::function<void(bool)> callback)
    {
        BedroomPartyLB::WebUtils::PostAsync(url, requestBody, false, [callback](std::string value, bool success, int responseCode)
        {
            tryAgain = responseCode != HttpStatusCode::Conflict;
            callback(success); 
        });
    }
}