#pragma once

#include "UnityEngine/EventSystems/IPointerEnterHandler.hpp"
#include "UnityEngine/EventSystems/IPointerExitHandler.hpp"
#include "UnityEngine/EventSystems/IEventSystemHandler.hpp"
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "TMPro/FontStyles.hpp"
#include "UnityEngine/GameObject.hpp"

#define INTERFACES \
    { \
        classof(UnityEngine::EventSystems::IPointerEnterHandler*), \
        classof(UnityEngine::EventSystems::IPointerExitHandler*), classof(UnityEngine::EventSystems::IEventSystemHandler*) \
    } \

___DECLARE_TYPE_WRAPPER_INHERITANCE(BedroomPartyLB::UI, TextHoverEffect, Il2CppTypeEnum::IL2CPP_TYPE_CLASS, UnityEngine::MonoBehaviour, "BedroomPartyLB::UI", INTERFACES, 0, nullptr,

    DECLARE_OVERRIDE_METHOD(void, OnPointerEnter, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::UnityEngine::EventSystems::IPointerEnterHandler::OnPointerEnter>::get());
    DECLARE_OVERRIDE_METHOD(void, OnPointerExit, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::UnityEngine::EventSystems::IPointerExitHandler::OnPointerExit>::get());
    DECLARE_STATIC_METHOD(void, AddEffect, TMPro::TextMeshProUGUI* text, TMPro::FontStyles hover, TMPro::FontStyles orig);

    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, component);
    DECLARE_INSTANCE_FIELD(TMPro::FontStyles, daStyle);
    DECLARE_INSTANCE_FIELD(TMPro::FontStyles, original);
)

#undef INTERFACES