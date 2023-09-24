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

    inline const std::string format_float(float num)
    {
        std::string stringified = string_format("%.2f", num);
        auto found = stringified.find_last_not_of('0');
        if (found != std::string::npos)
        {
            if (stringified.at(found) == '.') found--;
            stringified.erase(found + 1);
        }
        return stringified;
    }

    inline const std::string banishToOz(const std::string& name)
    {
        return "<rotate=180>" + std::string(name.rbegin(), name.rend()) + "</rotate>";
    }
}