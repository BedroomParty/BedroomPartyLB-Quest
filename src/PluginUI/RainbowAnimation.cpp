#include "PluginUI/RainbowAnimation.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Time.hpp"

DEFINE_TYPE(BedroomPartyLB::UI, RainbowAnimation);

namespace BedroomPartyLB::UI
{
    using Colour = UnityEngine::Color;
    using namespace UnityEngine;

    void RainbowAnimation::Start()
    {
        text = GetComponent<TMPro::TextMeshProUGUI*>();
        speed = 0.4f;
    }

    void RainbowAnimation::Update()
    {
        if (text == nullptr) return;

        hue += speed * Time::get_deltaTime();
        if (hue > 1.0f) hue -= 1.0f;

        Colour rainbowColour = Colour::HSVToRGB(hue, 1.0f, 1.0f);
        text->set_color(rainbowColour);
    }

    void RainbowAnimation::OnDestroy()
    {
        text->set_color(Colour::get_white());
    }
}