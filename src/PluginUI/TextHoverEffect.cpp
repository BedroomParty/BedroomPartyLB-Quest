#include "PluginUI/TextHoverEffect.hpp"
#include "Utils/Constants.hpp"

DEFINE_TYPE(BedroomPartyLB::UI, TextHoverEffect);

namespace BedroomPartyLB::UI 
{

    void TextHoverEffect::AddEffect(TMPro::TextMeshProUGUI* text, TMPro::FontStyles hover, TMPro::FontStyles orig) 
    {
        if (text == nullptr) return;
        UnityEngine::Object::Destroy(text->get_gameObject()->GetComponent<TextHoverEffect*>());
        TextHoverEffect* effect = text->get_gameObject()->AddComponent<TextHoverEffect*>();
        effect->component = text;
        effect->daStyle = hover;
        effect->original = orig;
    }

    void TextHoverEffect::OnPointerEnter()
    {
        component->set_fontStyle(daStyle);
    }

    void TextHoverEffect::OnPointerExit()
    {
        component->set_fontStyle(original);
    }
}