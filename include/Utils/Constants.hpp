#pragma once

#include "UnityEngine/Color.hpp"
#include <string>

namespace BedroomPartyLB::Constants{
    static const UnityEngine::Color BP_COLOR = UnityEngine::Color(0.674509804f, 0.760784314f, 0.850980392f, 1.0f);
    static const UnityEngine::Color BP_COLOR2 = UnityEngine::Color(0.839215686f, 0.705882353f, 0.988235294f, 1.0f);
    static const std::string USER_PROFILE_LINK = "https://thebedroom.party?user=";
    static const std::string SCARY_FILE = "/sdcard/ModData/com.beatgames.beatsaber/Mods/BPLB/DO_NOT_SHARE.scary";
    static const std::string BASE_URL = "https://dev.thebedroom.party/";
    static const std::string BASE_WEB_URL = "https://thebedroom.party";
    static const std::string BUG_REPORT = "/bug-report";
    static const std::string LEADERBOARD = "/leaderboard/";
}