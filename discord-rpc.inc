#if defined _discord_rpc_included
    #endinput
#endif
#define _discord_rpc_included

#tryinclude <Pawn.RakNet>

#if !defined PAWNRAKNET_INC_
    #error Pawn.RakNet is needed in this include.
#endif

#if __pawn_build >= 11
    forward OnPlayerRequestDiscordRichPresence(playerid);
#else
    forward OnPlayerRequestDiscordRPC(playerid);
#endif

#if !defined strcopy
    stock strcopy(dest[], const source[], maxlength=sizeof dest)
        return strcat((dest[0] = EOS, dest), source, maxlength);
#endif

static const PRESENCE_RPC = 240;

enum _:PresenceAction 
{
    PresenceAction_Init = 0,
    PresenceAction_AppID,
    PresenceAction_AssetLarge,
    PresenceAction_AssetSmall,
    PresenceAction_State,
    PresenceAction_Details,
    PresenceAction_Buttons,
    PresenceAction_ForceUpdate
};

// Internal
ICustomRPC:PRESENCE_RPC(playerid, BitStream:bs)
{
    #if __pawn_build >= 11
        CallLocalFunction("OnPlayerRequestDiscordRichPresence", "i", playerid);
    #else
        CallLocalFunction("OnPlayerRequestDiscordRPC", "i", playerid);
    #endif
}

// API
stock DiscordRPC_SetApplicationID(playerid, const id[], bool:updateImmediately = false)
{
    new BitStream:bs = BS_New();

    BS_WriteUint8(bs, PresenceAction_AppID);
    BS_WriteBool(bs, updateImmediately);
    BS_WriteString32(bs, id);
    
    new ret = PR_SendRPC(bs, playerid, PRESENCE_RPC);

    BS_Delete(bs);
    return ret;
}

stock DiscordRPC_SetAsset(playerid, const asset[], const assetText[], bool:isLarge, bool:updateImmediately = true)
{
    new BitStream:bs = BS_New();

    BS_WriteUint8(bs, isLarge ? PresenceAction_AssetLarge : PresenceAction_AssetSmall);
    BS_WriteBool(bs, updateImmediately);
    BS_WriteString32(bs, asset);
    BS_WriteString32(bs, assetText);

    new ret = PR_SendRPC(bs, playerid, PRESENCE_RPC);

    BS_Delete(bs);
    return ret;
}

stock DiscordRPC_SetLargeAsset(playerid, const asset[], const assetText[], bool:updateImmediately = false)
{
    return DiscordRPC_SetAsset(playerid, asset, assetText, true /* isLarge */, updateImmediately);
}

stock DiscordRPC_SetSmallAsset(playerid, const asset[], const assetText[], bool:updateImmediately = false)
{
    return DiscordRPC_SetAsset(playerid, asset, assetText, false /* isLarge */, updateImmediately);
}

stock DiscordRPC_SetState(playerid, const stateText[], bool:updateImmediately = false)
{
    new BitStream:bs = BS_New();

    BS_WriteUint8(bs, PresenceAction_State);
    BS_WriteBool(bs, updateImmediately);
    BS_WriteString32(bs, stateText);

    new ret = PR_SendRPC(bs, playerid, PRESENCE_RPC);

    BS_Delete(bs);
    return ret;
}

stock DiscordRPC_SetDetails(playerid, const details[], bool:updateImmediately = false)
{
    new BitStream:bs = BS_New();

    BS_WriteUint8(bs, PresenceAction_Details);
    BS_WriteBool(bs, updateImmediately);
    BS_WriteString32(bs, details);

    new ret = PR_SendRPC(bs, playerid, PRESENCE_RPC);

    BS_Delete(bs);
    return ret;
}

stock DiscordRPC_SetButtons(playerid, index, const button[], const buttonUrl[], bool:updateImmediately = false)
{
    new BitStream:bs = BS_New();

    BS_WriteUint8(bs, PresenceAction_Buttons);
    BS_WriteBool(bs, updateImmediately);
    BS_WriteUint8(bs, index);
    BS_WriteString32(bs, button);
    BS_WriteString32(bs, buttonUrl);

    new ret = PR_SendRPC(bs, playerid, PRESENCE_RPC);

    BS_Delete(bs);
    return ret;
}

stock DiscordRPC_SetButton1(playerid, const button[], const buttonUrl[], bool:updateImmediately = false)
{
    return DiscordRPC_SetButtons(playerid, 1, button, buttonUrl, updateImmediately);
}

stock DiscordRPC_SetButton2(playerid, const button[], const buttonUrl[], bool:updateImmediately = false)
{
    return DiscordRPC_SetButtons(playerid, 2, button, buttonUrl, updateImmediately);
}

stock DiscordRPC_ForceUpdate(playerid)
{
    new BitStream:bs = BS_New();
    BS_WriteUint8(bs, PresenceAction_ForceUpdate);
    new ret = PR_SendRPC(bs, playerid, PRESENCE_RPC);
    BS_Delete(bs);
    return ret;
}

stock DiscordRPC_InitData(playerid, const applicationId[], const stateText[], const details[], const largeAsset[] = "", const largeAssetText[] = "", const smallAsset[] = "", const smallAssetText[] = "", const button1[] = "", const button1Url[] = "", const button2[] = "", const button2Url[] = "")
{
    new BitStream:bs = BS_New();
    BS_WriteUint8(bs, PresenceAction_Init);
    BS_WriteString32(bs, applicationId);
    BS_WriteString32(bs, stateText);
    BS_WriteString32(bs, details);
    BS_WriteString32(bs, largeAsset);
    BS_WriteString32(bs, largeAssetText);
    BS_WriteString32(bs, smallAsset);
    BS_WriteString32(bs, smallAssetText);
    BS_WriteString32(bs, button1);
    BS_WriteString32(bs, button1Url);
    BS_WriteString32(bs, button2);
    BS_WriteString32(bs, button2Url);

    new ret = PR_SendRPC(bs, playerid, PRESENCE_RPC);

    BS_Delete(bs);
    return ret;
}