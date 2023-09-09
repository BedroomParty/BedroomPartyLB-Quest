#include "System/Collections/IEnumerator.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"
#include "main.hpp"
#include "Utils/WebUtils.hpp"

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

using namespace UnityEngine::Networking;
using namespace std;
using namespace UnityEngine;

#define coro(coroutine) GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::CoroutineHelper::New(coroutine))

namespace BedroomPartyLB::WebUtils
{
    custom_types::Helpers::Coroutine PostAsyncCoroutine(string url, string body, bool isLogin, function<void(string, bool)> callback)
    {        
        auto request = UnityWebRequest::New_ctor(url, "POST", DownloadHandlerBuffer::New_ctor(), UploadHandlerRaw::New_ctor(System::Text::Encoding::get_UTF8()->GetBytes(body)));
        request->SetRequestHeader("Content-Type", "application/json");
        if (isLogin)
            request->SetRequestHeader("Authorization", apiKey);
        else
            request->SetRequestHeader("Authorization", sessionKey);
        DEBUG("{}", sessionKey);

        co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(request->SendWebRequest()));

        if (!request->get_isNetworkError() || !request->get_isHttpError())
            callback(request->get_downloadHandler()->GetText(), true);
        else
            callback("", false);

        request->Dispose();
        co_return;
    }

    custom_types::Helpers::Coroutine GetAsyncCoroutine(string url, function<void(string, bool)> callback)
    {
        auto request = UnityWebRequest::Get(url);
        request->SetRequestHeader("Content-Type", "application/json");

        co_yield reinterpret_cast<System::Collections::IEnumerator*>(CRASH_UNLESS(request->SendWebRequest()));
        if (!request->get_isNetworkError() && !request->get_isHttpError())
            callback(request->get_downloadHandler()->GetText(), true);
        else
            callback("", false);

        request->Dispose();
        co_return;
    }

    void PostAsync(string url, string body, bool isLogin, function<void (string, bool)> callback)
    {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::new_coro(PostAsyncCoroutine(url, body, isLogin, callback)));
    }

    void GetAsync(string url, function<void(string, bool)> callback)
    {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(custom_types::Helpers::new_coro(GetAsyncCoroutine(url, callback)));
    }

    void GetImageAsync(std::string URL, std::function<void(Sprite*)> callback){
        Lapiz::Utilities::MainThreadScheduler::Schedule([=](){
            auto request = UnityWebRequestTexture::GetTexture(URL);
            request->SetRequestHeader("User-Agent", std::string(MOD_ID) + " " + VERSION);
            request->SendWebRequest()->add_completed(DLCompletedDeleg([=](auto* value){
                if (request->get_isHttpError() || request->get_isNetworkError()) return callback(nullptr);
                auto downloadHandlerTexture = reinterpret_cast<DownloadHandlerTexture*>(request->get_downloadHandler());
                auto texture = downloadHandlerTexture->get_texture();
                if (texture == nullptr) return callback(nullptr);
                auto sprite = Sprite::Create(texture, 
                                            Rect(0.0f, 0.0f, 
                                            (float)texture->get_width(), 
                                            (float)texture->get_height()), 
                                            Vector2(0.5f, 0.5f), 
                                            1024.0f, 
                                            1u, 
                                            SpriteMeshType::FullRect, 
                                            Vector4(0.0f, 0.0f, 0.0f, 0.0f), 
                                            false);
                callback(sprite);
            }));
        });
    }
}