#include <mutex>
#include <Windows.h>

#include "discord.hpp"

namespace Discord
{
    std::string discordAppId;
    std::string discordAppAsset;
    std::string discordAppAssetText;
    std::string discordAppAssetSmall;
    std::string discordAppAssetSmallText;

    std::string discordAppCurrentId;
    std::string discordAppState;
    std::string discordAppDetails;

    std::mutex threadSafety;

    time_t discordAppStart;
    TimePoint lastUpdateTime = std::chrono::steady_clock::time_point(Milliseconds(0));
    bool shouldUpdate = false;
    bool discordInitialized = false;

    bool isUsingOmp = false;
    
    std::optional<std::tuple<std::pair<std::string, std::string>, std::pair<std::string, std::string>>> buttons_;

    void SetDefaultData()
    {
        discordAppId = DEFAULT_SAMP_APP_ID;
        discordAppAsset = DEFAULT_SAMP_APP_ASSET;
        discordAppAssetText = DEFAULT_SAMP_APP_ASSET_TEXT;

        discordAppAssetSmall = DEFAULT_SAMP_APP_ASSET_SMALL;
        discordAppAssetSmallText = DEFAULT_SAMP_APP_ASSET_SMALL_TEXT;

        discordAppCurrentId = DEFAULT_SAMP_APP_ID;
        discordAppDetails.clear();
        discordAppState.clear();

        discordAppStart = std::time(0);
        shouldUpdate = true;

        buttons_ = {};
    }

    static void Ready(const DiscordUser* discordUser) {}
    static void Errored(int errorCode, const char* message) {}
    static void Disconnected(int errorCode, const char* message) {}
    static void JoinGame(const char* joinSecret) {}
    static void SpectateGame(const char* spectateSecret) {}
    static void JoinRequest(const DiscordUser* request) {}

    void Initialize()
    {
        if (discordInitialized)
        {
            return;
        }
        
        std::lock_guard<std::mutex> lock(threadSafety);
        DiscordEventHandlers handlers;
        memset(&handlers, 0, sizeof(handlers));

        handlers.ready = Ready;
        handlers.errored = Errored;
        handlers.disconnected = Disconnected;
        
        Discord_Initialize((discordAppCurrentId.empty()) ? DEFAULT_SAMP_APP_ID : discordAppCurrentId.c_str(), &handlers, 1, nullptr);
        discordInitialized = true;
    }

    void Restart()
    {
        if (!discordInitialized)
        {
            Initialize();
            return;
        }
        
        Shutdown();

        for (int i = 0; i < 100 && Discord_IsConnected(); ++ i)
        {
            Discord_RunCallbacks();
            Sleep(10);
        }

        discordInitialized = false;
        Initialize();
    }

    void Shutdown()
    {
        if (!discordInitialized)
        {
            return;
        }

        Discord_ClearPresence();
        Discord_Shutdown();
        discordInitialized = false;
    }

    void Reset()
    {
        const bool appChanged = discordAppCurrentId != DEFAULT_SAMP_APP_ID;

        SetDefaultData();
        if (appChanged)
        {
            Restart();
        }

        shouldUpdate = true;
        Update();
    }

    void Poll()
    {
        if (discordInitialized)
        {
            Discord_RunCallbacks();
        }
    }

    void Update(std::string state, std::string details, std::string largeAsset, std::string largeText, std::string smallAsset, std::string smallText, std::string button1, std::string button1Url, std::string button2, std::string button2Url)
    {
        SetPresenceState(state.c_str());
        SetPresenceDetails(details.c_str());
        SetAssetLargeData(largeAsset.c_str(), largeText.c_str());
        SetAssetSmallData(smallAsset.c_str(), smallText.c_str());
        SetPresenceButtons(1, button1.c_str(), button1Url.c_str());
        SetPresenceButtons(2, button2.c_str(), button2Url.c_str());
        
        shouldUpdate = true;
        return Update();
    }

    void Update()
    {
        if (!shouldUpdate || !discordInitialized)
        {
            return;
        }

        std::lock_guard<std::mutex> lock(threadSafety);
        DiscordRichPresence presence;
        memset(&presence, 0, sizeof(presence));

        presence.state = discordAppState.c_str();
        presence.details = discordAppDetails.c_str();
        presence.startTimestamp = discordAppStart;
        presence.largeImageKey = discordAppAsset.c_str();
        presence.largeImageText = discordAppAssetText.c_str();
        presence.smallImageKey = discordAppAssetSmall.c_str();
        presence.smallImageText = discordAppAssetSmallText.c_str();

        if (buttons_)
        {
            presence.button1name = std::get<0>(*buttons_).first.c_str();
            presence.button1link = std::get<0>(*buttons_).second.c_str();

            presence.button2name = std::get<1>(*buttons_).first.c_str();
            presence.button2link = std::get<1>(*buttons_).second.c_str();
        }

        Discord_UpdatePresence(&presence);
        shouldUpdate = false;
    }

    void SetDiscordAppId(const char* appId)
    {
        std::string newId = (appId && *appId) ? appId : DEFAULT_SAMP_APP_ID;

        if (!discordInitialized)
        {
            discordAppCurrentId = newId;
            Initialize();
            shouldUpdate = true;
            return;
        }

        if (newId == discordAppCurrentId)
        {
            shouldUpdate = true;
            return;
        }
        discordAppCurrentId = newId;
        Restart();
        shouldUpdate = true;
    }

    const char* GetDiscordAppId()
    {
        return (discordAppCurrentId.empty()) ? DEFAULT_SAMP_APP_ID : discordAppCurrentId.c_str();
    }

    void SetAssetLargeData(const char* asset, const char* assetText)
    {
        SetAsset(asset, assetText, true);
    }

    void SetAssetSmallData(const char* asset, const char* assetText)
    {
        SetAsset(asset, assetText, false);
    }

    void SetAsset(const char* asset, const char* assetText, bool isLarge)
    {
        if (isLarge)
        {
            discordAppAsset = (std::strlen(asset) > 0 && asset && *asset) ? asset : DEFAULT_SAMP_APP_ASSET;
            discordAppAssetText = (std::strlen(assetText) > 0 && assetText && *assetText) ? assetText : DEFAULT_SAMP_APP_ASSET_TEXT;
        }
        else
        {
            discordAppAssetSmall = (std::strlen(asset) > 0 && asset && *asset) ? asset : DEFAULT_SAMP_APP_ASSET_SMALL;
            discordAppAssetSmallText = (std::strlen(assetText) > 0 && assetText && *assetText) ? assetText : DEFAULT_SAMP_APP_ASSET_SMALL_TEXT;
        }
        shouldUpdate = true;
    }

    void SetPresenceState(const char* state)
    {
        discordAppState = state;
        shouldUpdate = true;
    }

    void SetPresenceDetails(const char* details)
    {
        discordAppDetails = details;
        shouldUpdate = true;
    }

    void SetPresenceButtons(unsigned short int index, const char* name, const char* url)
    {
        if ((index <= 0 || index > 2))
        {
            return;
        }

        std::decay_t<decltype(*buttons_)> buttons;
        if (buttons_)
        {
            buttons = *buttons_;
        }

        if (index == 1)
        {
            std::get<0>(buttons) = {name, url};
        }
        else if (index == 2)
        {
            std::get<1>(buttons) = {name, url};
        }
        buttons_ = buttons;
        shouldUpdate = true;
    }

    void SetPresenceStartTimestamp(time_t start)
    {
        discordAppStart = start;
        shouldUpdate = true;
    }

    void SetLastUpdateTime(TimePoint lastUpdateTime)
    {
        Discord::lastUpdateTime = lastUpdateTime;
    }

    TimePoint GetLastUpdateTime()
    {
        return lastUpdateTime;
    }

    void SetShouldUpdate(bool status)
    {
        shouldUpdate = status;
    }

    bool ShouldUpdate()
    {
        return shouldUpdate;
    }
}