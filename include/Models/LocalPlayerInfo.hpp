#pragma once

#include <string>

namespace BedroomPartyLB::Models{
    struct LocalPlayerInfo {
        std::string userID;
        std::string username;
        std::string apiKey;
        std::string avatarLink;
        std::string sessionKey;
    };
}
extern BedroomPartyLB::Models::LocalPlayerInfo localPlayerInfo;