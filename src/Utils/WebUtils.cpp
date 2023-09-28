#include "System/Collections/IEnumerator.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"
#include "main.hpp"
#include "Utils/WebUtils.hpp"
#include "EasyDelegate.hpp"
#include "bsml/shared/Helpers/utilities.hpp"

#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"

#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/Networking/DownloadHandlerBuffer.hpp"
#include "UnityEngine/Networking/UploadHandlerRaw.hpp"
#include "UnityEngine/Networking/UnityWebRequestTexture.hpp"
#include "UnityEngine/Networking/DownloadHandlerTexture.hpp"
#include "UnityEngine/Networking/UnityWebRequestAsyncOperation.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Vector4.hpp"

#include "System/Text/Encoding.hpp"
#include <string>
#include <optional>
#include <functional>
#include "Models/LocalPlayerInfo.hpp"
#include "UnityEngine/Networking/UnityWebRequest_UnityWebRequestError.hpp"

using namespace UnityEngine::Networking;
using namespace std;
using namespace UnityEngine;
using namespace EasyDelegate;

#define coro(coroutine) GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(coroutine))

namespace BedroomPartyLB::WebUtils
{
    custom_types::Helpers::Coroutine PostAsyncCoroutine(string url, string body, bool isLogin, function<void(string, bool, int)> callback)
    {        
        auto request = UnityWebRequest::New_ctor(url, "POST", DownloadHandlerBuffer::New_ctor(), UploadHandlerRaw::New_ctor(System::Text::Encoding::get_UTF8()->GetBytes(body)));
        request->SetRequestHeader("Content-Type", "application/json");
        if (isLogin)
            request->SetRequestHeader("Authorization", localPlayerInfo.apiKey);
        else
            request->SetRequestHeader("Authorization", localPlayerInfo.sessionKey);
        DEBUG("{}", localPlayerInfo.sessionKey);

        co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(request->SendWebRequest()));
        callback(request->get_downloadHandler()->GetText(), !request->get_isNetworkError() && !request->get_isHttpError(), request->get_responseCode());

        request->Dispose();
        co_return;
    }

    custom_types::Helpers::Coroutine GetAsyncCoroutine(string url, function<void(string, bool)> callback)
    {
        auto request = UnityWebRequest::Get(url);
        request->SetRequestHeader("Content-Type", "application/json");

        co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(request->SendWebRequest()));
        
        callback(request->get_downloadHandler()->GetText(), !request->get_isNetworkError() && !request->get_isHttpError());

        request->Dispose();
        co_return;
    }

    void PostAsync(string url, string body, bool isLogin, function<void (string, bool, int)> callback)
    {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::new_coro(PostAsyncCoroutine(url, body, isLogin, callback)));
    }

    void GetAsync(string url, function<void(string, bool)> callback)
    {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::new_coro(GetAsyncCoroutine(url, callback)));
    }

    void GetImageAsync(std::string URL, std::function<void(Sprite*)> callback)
    {
        using DLFinish = System::Action_1<AsyncOperation*>;
        Lapiz::Utilities::MainThreadScheduler::Schedule([=]()
        {
            auto request = UnityWebRequestTexture::GetTexture(URL);
            request->SetRequestHeader("User-Agent", std::string(MOD_ID) + " " + VERSION);
            request->SendWebRequest()->add_completed(MakeDelegate<DLFinish*>([=](AsyncOperation* value)
            {
                if (request->get_isHttpError() || request->get_isNetworkError()) return callback(nullptr);
                auto downloadHandlerTexture = reinterpret_cast<DownloadHandlerTexture*>(request->get_downloadHandler());
                auto texture = downloadHandlerTexture->get_texture();
                if (texture == nullptr) return callback(nullptr);
                auto sprite = BSML::Utilities::LoadSpriteFromTexture(texture);
                callback(sprite);
                request->Dispose();
            }));
        });
    }
}