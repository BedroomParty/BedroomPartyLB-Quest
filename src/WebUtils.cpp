#include "System/Collections/IEnumerator.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "main.hpp"
#include "WebUtils.hpp"

#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"

#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/Networking/DownloadHandlerBuffer.hpp"
#include "UnityEngine/Networking/UploadHandlerRaw.hpp"

#include "System/Text/Encoding.hpp"
#include <string>
#include <optional>
#include <functional>

using namespace UnityEngine::Networking;
using namespace std;

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

        if (!request->get_isNetworkError() || !request->get_isHttpError())
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
}