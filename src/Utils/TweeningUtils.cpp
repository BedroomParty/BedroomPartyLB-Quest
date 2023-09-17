#include "Utils/TweeningUtils.hpp"
#include "UnityEngine/Resources.hpp"
#include "Tweening/FloatTween.hpp"
#include "Tweening/Tween.hpp"
#include "GlobalNamespace/EaseType.hpp"
#include "UnityEngine/GameObject.hpp"
#include "EasyDelegate.hpp"
#include "System/Action_1.hpp"
#include "System/Action.hpp"

namespace BedroomPartyLB::TweeningUtils{
    using namespace Tweening;
    using namespace UnityEngine;
    using namespace TMPro;
    using namespace GlobalNamespace;
    using namespace EasyDelegate;
    using namespace System;

    using Colour = UnityEngine::Color;

    TimeTweeningManager* getTweeningManager(){
        static SafePtrUnity<TimeTweeningManager> tweeningManager;
        if (tweeningManager) return tweeningManager.ptr();
        tweeningManager = Resources::FindObjectsOfTypeAll<TimeTweeningManager*>().FirstOrDefault();
        return tweeningManager.ptr();
    }

    void FadeText(TextMeshProUGUI* text, bool fadeIn, float time)
    {
        if (text == nullptr || text->m_CachedPtr.m_value == nullptr) return;
        float startAlpha = fadeIn ? 0.0f : 1.0f;
        float endAlpha = fadeIn ? 1.0f : 0.0f;

        Tween* tween = FloatTween::New_ctor(startAlpha, endAlpha, MakeDelegate<Action_1<float>*>([text](float u)
        {
            Colour colour = text->get_color();
            colour.a = u;
            text->set_color(colour);
        }), 0.4f, EaseType::Linear, 0.0f);

        tween->onCompleted = MakeDelegate<Action*>([fadeIn, text]()
        {
            if (text == nullptr) return;
            text->get_gameObject()->SetActive(fadeIn);
        });

        tween->onKilled = MakeDelegate<Action*>([fadeIn, text, endAlpha]()
        {
            if (text == nullptr) return;
            text->get_gameObject()->SetActive(fadeIn);
            Colour colour = text->get_color();
            colour.a = endAlpha;
            text->set_color(colour);
        });
  
        text->get_gameObject()->SetActive(true);
        getTweeningManager()->AddTween(tween, text);
    }
}