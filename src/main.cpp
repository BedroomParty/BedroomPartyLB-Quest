#include "main.hpp"
#include "Hooks.hpp"
#include "assets.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "Models/CustomLeaderboard.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/utils-functions.h"
#include "bsml/shared/BSML.hpp"
#include "custom-types/shared/register.hpp"
#include "leaderboardcore/shared/LeaderboardCore.hpp"
#include "bsml/shared/BSMLDataCache.hpp"
#include "Models/LocalPlayerInfo.hpp"

ModInfo modInfo;
BedroomPartyLB::Models::CustomLeaderboard leaderboard;
BedroomPartyLB::Models::LocalPlayerInfo localPlayerInfo;

Configuration& getConfig()
{
    static Configuration config(modInfo);
    return config;
}

Logger& getLogger()
{
    static Logger *logger = new Logger(modInfo, {false, true});
    return *logger;
}

extern "C" void setup(ModInfo &info)
{
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;

    getConfig().Load();
    getLogger().info("Completed setup!");
}

void LeaderboardSet(GlobalNamespace::IDifficultyBeatmap *difficultyBeatmap)
{
    leaderboard.get_leaderboardViewController()->page = 0;
    leaderboard.get_leaderboardViewController()->CheckPage();
    leaderboard.currentDifficultyBeatmap = difficultyBeatmap;
    leaderboard.get_leaderboardViewController()->RefreshLeaderboard(difficultyBeatmap);
}

extern "C" void load()
{
    il2cpp_functions::Init();
    BSML::Init();
    custom_types::Register::AutoRegister();
    LeaderboardCore::Register::RegisterLeaderboard(&leaderboard, modInfo);
    LeaderboardCore::Events::NotifyLeaderboardSet() += LeaderboardSet;
    Hooks::InstallHooks(getLogger());
}

BSML_DATACACHE(Arrow_png)
{
    return IncludedAssets::Arrow_png;
}

BSML_DATACACHE(Blank_png)
{
    return IncludedAssets::Blank_png;
}

BSML_DATACACHE(Bug_png)
{
    return IncludedAssets::Bug_png;
}

BSML_DATACACHE(Globe_png)
{
    return IncludedAssets::Globe_png;
}

BSML_DATACACHE(Info_png)
{
    return IncludedAssets::Info_png;
}

BSML_DATACACHE(Link_png)
{
    return IncludedAssets::Link_png;
}

BSML_DATACACHE(Lmao_png)
{
    return IncludedAssets::Lmao_png;
}

BSML_DATACACHE(Logo_png)
{
    return IncludedAssets::Logo_png;
}
BSML_DATACACHE(Pixel_png)
{
    return IncludedAssets::Pixel_png;
}
BSML_DATACACHE(Player_png)
{
    return IncludedAssets::Player_png;
}
BSML_DATACACHE(pp_png)
{
    return IncludedAssets::pp_png;
}