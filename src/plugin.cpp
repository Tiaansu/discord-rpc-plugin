#include <RakNet/PacketEnumerations.h>
#include <RakNet/StringCompressor.h>

#include "plugin.hpp"
#include "detail.hpp"
#include "discord.hpp"
#include "types.hpp"

void Tick()
{
    Discord::Poll();
    
    auto cooldown = std::chrono::steady_clock::duration(Milliseconds(Discord::TIME_DISCORD_UPDATE_RICH_PRESENCE_RATE));

    const TimePoint now = Time::now();
    const TimePoint lastUpdate = Discord::GetLastUpdateTime();
    if ((now - lastUpdate) >= cooldown && Discord::ShouldUpdate())
    {
        Discord::Update();
        Discord::SetLastUpdateTime(Time::now());
    }
}

void CPlugin::GameLoop()
{
    static bool initialized = false;

    Tick();

    if (rakhook::samp_version() == rakhook::samp_ver::v037r2 || rakhook::samp_version() == rakhook::samp_ver::v037r4)
    {
        GameLoopHook.CallOriginal();
        GameLoopHook.Remove();
        MessageBox(nullptr, "Unsupported SAMP version: 0.3.7-R2, 0.3.7-R4", "Error", MB_OK | MB_ICONERROR);
        exit(0);
        return;
    }

    if (initialized || !rakhook::initialize())
    {
        return GameLoopHook.CallOriginal();
    }

    initialized = true;
    StringCompressor::AddReference();

    Discord::Initialize();

    rakhook::on_receive_packet +=
        [](Packet *packet) -> bool
        {
            if (!packet || packet->length == 0 || packet->data == nullptr)
            {
                return true;
            }

            unsigned char id = packet->data[0];
            switch (id)
            {
                case ID_DISCONNECTION_NOTIFICATION:
                case ID_CONNECTION_LOST:
                case ID_CONNECTION_BANNED:
                case ID_CONNECTION_ATTEMPT_FAILED:
                case ID_NO_FREE_INCOMING_CONNECTIONS:
                case ID_INVALID_PASSWORD:
                {
                    Discord::Reset();
                    break;
                }
            }
            return true;
        };

    rakhook::on_receive_rpc +=
        [](unsigned char& id, RakNet::BitStream* bs) -> bool
        {
            switch (id)
            {
                case 139: // InitGame
                {
                    rakhook::send_rpc(Discord::RPC_ID::PRESENCE_RPC, bs, PacketPriority::HIGH_PRIORITY, PacketReliability::RELIABLE_ORDERED, 0, true);
                    break;
                }
                case Discord::RPC_ID::PRESENCE_RPC:
                {
                    unsigned char action;
                    bs->Read(action);

                    switch (action)
                    {
                        case Discord::PresenceAction::ACTION_INIT:
                        {
                            std::string appId = read_with_size<unsigned int>(bs);
                            std::string state = read_with_size<unsigned int>(bs);
                            std::string details = read_with_size<unsigned int>(bs);
                            std::string assetLarge = read_with_size<unsigned int>(bs);
                            std::string assetLargeText = read_with_size<unsigned int>(bs);
                            std::string assetSmall = read_with_size<unsigned int>(bs);
                            std::string assetSmallText = read_with_size<unsigned int>(bs);
                            std::string button1 = read_with_size<unsigned int>(bs);
                            std::string button1Url = read_with_size<unsigned int>(bs);
                            std::string button2 = read_with_size<unsigned int>(bs);
                            std::string button2Url = read_with_size<unsigned int>(bs);

                            Discord::SetPresenceStartTimestamp(time(0));
                            Discord::SetDiscordAppId(appId.c_str());
                            Discord::Update(state, details, assetLarge, assetLargeText, assetSmall, assetSmallText, button1, button1Url, button2, button2Url);

                            break;
                        }
                        case Discord::PresenceAction::ACTION_APP_ID:
                        {
                            bool updateImmediately;
                            bs->Read(updateImmediately);

                            std::string appId = read_with_size<unsigned int>(bs);

                            Discord::SetPresenceStartTimestamp(time(0));
                            Discord::SetDiscordAppId(appId.c_str());

                            if (updateImmediately)
                            {
                                Discord::Update();
                            }
                            break;
                        }
                        case Discord::PresenceAction::ACTION_ASSET_LARGE:
                        {
                            bool updateImmediately;
                            bs->Read(updateImmediately);
                            
                            std::string asset = read_with_size<unsigned int>(bs);
                            std::string assetText = read_with_size<unsigned int>(bs);

                            Discord::SetAssetLargeData(asset.c_str(), assetText.c_str());
                            if (updateImmediately)
                            {
                                Discord::Update();
                            }
                            break;
                        }
                        case Discord::PresenceAction::ACTION_ASSET_SMALL:
                        {
                            bool updateImmediately;
                            bs->Read(updateImmediately);
                            
                            std::string asset = read_with_size<unsigned int>(bs);
                            std::string assetText = read_with_size<unsigned int>(bs);

                            Discord::SetAssetSmallData(asset.c_str(), assetText.c_str());
                            if (updateImmediately)
                            {
                                Discord::Update();
                            }
                            break;
                        }
                        case Discord::PresenceAction::ACTION_STATE:
                        {
                            bool updateImmediately;
                            bs->Read(updateImmediately);
                            
                            std::string state = read_with_size<unsigned int>(bs);

                            Discord::SetPresenceState(state.c_str());
                            if (updateImmediately)
                            {
                                Discord::Update();
                            }
                            break;
                        }
                        case Discord::PresenceAction::ACTION_DETAILS:
                        {
                            bool updateImmediately;
                            bs->Read(updateImmediately);
                            
                            std::string details = read_with_size<unsigned int>(bs);

                            Discord::SetPresenceDetails(details.c_str());
                            if (updateImmediately)
                            {
                                Discord::Update();
                            }
                            break;
                        }
                        case Discord::PresenceAction::ACTION_BUTTONS:
                        {
                            bool updateImmediately;
                            bs->Read(updateImmediately);
                            
                            unsigned char index;
                            bs->Read(index);

                            if ((index <= 0 || index > 2))
                            {
                                break;
                            }

                            std::string name = read_with_size<unsigned int>(bs);
                            std::string url = read_with_size<unsigned int>(bs);

                            Discord::SetPresenceButtons(index, name.c_str(), url.c_str());
                            if (updateImmediately)
                            {
                                Discord::Update();
                            }
                            break;
                        }
                        case Discord::PresenceAction::ACTION_FORCE_UPDATE:
                        {
                            Discord::SetShouldUpdate(true);
                            Discord::Update();
                            break;
                        }
                    }
                }
            }
            return true;
        };
    return GameLoopHook.CallOriginal();
}

CPlugin::CPlugin(HMODULE module) : module_(module)
{
    GameLoopHook.Add(&CPlugin::GameLoop);
}

CPlugin::~CPlugin()
{
    rakhook::destroy();
}