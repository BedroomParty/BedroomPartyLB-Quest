#pragma once

#include "Tweening/TimeTweeningManager.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

namespace BedroomPartyLB::TweeningUtils
{
    Tweening::TimeTweeningManager* getTweeningManager();
    void FadeText(TMPro::TextMeshProUGUI* text, bool fadeIn, float time);
}