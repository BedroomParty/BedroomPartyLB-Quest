#pragma once

#include <string>
#include "rapidjson-macros/shared/macros.hpp"

namespace BedroomPartyLB::Models{
    DECLARE_JSON_CLASS(LocalPlayerInfo,
        NAMED_VALUE_DEFAULT(std::string, userID, "", "game_id");
        NAMED_VALUE_DEFAULT(std::string, discordID, "", "discord_id");
        NAMED_VALUE_DEFAULT(std::string, username, "null", "username");
        NAMED_VALUE_DEFAULT(std::string, avatarLink, "oops", "avatar");
        NAMED_VALUE_DEFAULT(std::string, sessionKey, "", "sessionKey");
        NAMED_VALUE_DEFAULT(long, sessionKeyExpires, 0, "sessionKeyExpires");

        public:
        std::string apiKey;
    )
}
extern BedroomPartyLB::Models::LocalPlayerInfo localPlayerInfo;