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

DECLARE_CLASS_CODEGEN_INTERFACES(BedroomPartyLB::UI, TextHoverEffect, UnityEngine::MonoBehaviour, std::vector<Il2CppClass*>(INTERFACES),

    DECLARE_OVERRIDE_METHOD(void, OnPointerEnter, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::UnityEngine::EventSystems::IPointerEnterHandler::OnPointerEnter>::get());
    DECLARE_OVERRIDE_METHOD(void, OnPointerExit, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::UnityEngine::EventSystems::IPointerExitHandler::OnPointerExit>::get());
    DECLARE_STATIC_METHOD(void, AddEffect, TMPro::TextMeshProUGUI* text, TMPro::FontStyles hover, TMPro::FontStyles orig, StringW normalText = "", StringW hoverText = "");

    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, component);
    DECLARE_INSTANCE_FIELD(TMPro::FontStyles, daStyle);
    DECLARE_INSTANCE_FIELD(TMPro::FontStyles, original);
    DECLARE_INSTANCE_FIELD(StringW, normalText);
    DECLARE_INSTANCE_FIELD(StringW, hoverText);
    DECLARE_INSTANCE_FIELD(bool, hasHoverText);
)

#undef INTERFACES