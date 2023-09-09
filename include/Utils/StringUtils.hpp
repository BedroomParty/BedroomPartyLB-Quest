#pragma once

#include "UnityEngine/Color.hpp"
#include <string>
#include "beatsaber-hook/shared/utils/utils-functions.h"

namespace BedroomPartyLB::StringUtils{
    inline const std::string rgbaToHex(UnityEngine::Color colour) {
        return string_format("#%02x%02x%02x%02x", 
            std::clamp<int>(colour.a * 255, 0, 255), 
            std::clamp<int>(colour.r * 255, 0, 255), 
            std::clamp<int>(colour.g * 255, 0, 255), 
            std::clamp<int>(colour.b * 255, 0, 255));
    }
}