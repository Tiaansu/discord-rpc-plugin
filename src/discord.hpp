#pragma once

#include <discord_rpc.h>
#include <string>
#include <optional>
#include <ctime>

#include "types.hpp"

namespace Discord
{
    // SAMP
    constexpr char DEFAULT_SAMP_APP_ID[] = "1324912836291330130";
    constexpr char DEFAULT_SAMP_APP_ASSET[] = "samp_logo";
    constexpr char DEFAULT_SAMP_APP_ASSET_TEXT[] = "Grand Theft Auto: San Andreas Multiplayer";
    constexpr char DEFAULT_SAMP_APP_ASSET_SMALL[] = "";
    constexpr char DEFAULT_SAMP_APP_ASSET_SMALL_TEXT[] = "";

    // open.mp
    constexpr char DEFAULT_OMP_APP_ID[] = "1327087051773972531";
    constexpr char DEFAULT_OMP_APP_ASSET[] = "omp_logo";
    constexpr char DEFAULT_OMP_APP_ASSET_TEXT[] = "Open Multiplayer";
    constexpr char DEFAULT_OMP_APP_ASSET_SMALL[] = "";
    constexpr char DEFAULT_OMP_APP_ASSET_SMALL_TEXT[] = "";
    
    constexpr long long TIME_DISCORD_UPDATE_RICH_PRESENCE_RATE = 10000;

    enum RPC_ID : int
    {
        INIT_PRESENCE = 240,
        SET_PRESENCE_APP_ID,
        SET_PRESENCE_ASSET_LARGE_DATA,
        SET_PRESENCE_ASSET_SMALL_DATA,
        SET_PRESENCE_STATE,
        SET_PRESENCE_DETAILS,
        SET_PRESENCE_BUTTONS,
        FORCE_UPDATE_PRESENCE
    };

    void Initialize();
    void Shutdown();
    void Restart();
    void SetDefaultData();

    void Update(std::string state, std::string details, std::string largeAsset, std::string largeText, std::string smallAsset, std::string smallText, std::string button1, std::string button1Url, std::string button2, std::string button2Url);
    void Update();

    void SetDiscordAppId(const char* appId);
    const char* GetDiscordAppId();
    void SetAssetLargeData(const char* asset, const char* assetText);
    void SetAssetSmallData(const char* asset, const char* assetText);
    void SetAsset(const char* asset, const char* assetText, bool isLarge);
    void SetPresenceState(const char* state);
    void SetPresenceDetails(const char* details);
    void SetPresenceButtons(unsigned short int index, const char* name, const char* url);
    void SetPresenceStartTimestamp(time_t start);

    void SetLastUpdateTime(TimePoint lastUpdateTime);
    TimePoint GetLastUpdateTime();
    void SetShouldUpdate(bool status);
    bool ShouldUpdate();
}