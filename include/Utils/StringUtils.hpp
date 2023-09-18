#pragma once

#include "UnityEngine/Color.hpp"
#include <string>
#include "beatsaber-hook/shared/utils/utils-functions.h"
#include <sstream>

namespace BedroomPartyLB::StringUtils
{
    inline const std::vector<std::string> split(const std::string &s, char delim)
    {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, delim)) elems.push_back(item);
        return elems;
    }
}