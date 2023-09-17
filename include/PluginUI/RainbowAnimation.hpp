#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "TMPro/TextMeshProUGUI.hpp"

DECLARE_CLASS_CODEGEN(BedroomPartyLB::UI, RainbowAnimation, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, Start);

    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, text);

    public:
    float speed;
    private:
    float hue;
)