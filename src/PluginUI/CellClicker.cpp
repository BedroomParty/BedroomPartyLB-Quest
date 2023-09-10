#include "PluginUI/CellClicker.hpp"
#include "Utils/Constants.hpp"

DEFINE_TYPE(BedroomPartyLB::UI, CellClicker);

namespace BedroomPartyLB::UI {

    void CellClicker::Start(){
        originalScale = separator->get_transform()->get_localScale();
    }

    void CellClicker::OnPointerEnter(){
        if (!isScaled)
        {
            separator->get_transform()->set_localScale(originalScale * 1.8f);
            isScaled = true;
        }
        separator->set_color(UnityEngine::Color::get_white());
        separator->set_color0(UnityEngine::Color::get_white());
        separator->set_color1(UnityEngine::Color(1, 1, 1, 0));
    }

    void CellClicker::OnPointerClick(){
        onClick();
    }

    void CellClicker::OnPointerExit(){
        if (isScaled)
        {
            separator->get_transform()->set_localScale(originalScale);
            isScaled = false;
        }
        separator->set_color(Constants::BP_COLOR2);
        separator->set_color0(UnityEngine::Color::get_white());
        separator->set_color1(UnityEngine::Color(1, 1, 1, 0));
    }

}