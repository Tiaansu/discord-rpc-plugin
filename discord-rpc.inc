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

static const 
    INIT_PRESENCE = 240,
    SET_PRESENCE_APP_ID = 241,
    SET_PRESENCE_ASSET_LARGE_DATA = 242,
    SET_PRESENCE_ASSET_SMALL_DATA = 243,
    SET_PRESENCE_STATE = 244,
    SET_PRESENCE_DETAILS = 245,
    SET_PRESENCE_BUTTONS = 246,
    FORCE_UPDATE_PRESENCE = 247;

// Internal
ICustomRPC:INIT_PRESENCE(playerid, BitStream:bs)
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

    BS_WriteBool(bs, updateImmediately);
    BS_WriteString32(bs, id);
    
    new ret = PR_SendRPC(bs, playerid, SET_PRESENCE_APP_ID);

    BS_Delete(bs);
    return ret;
}

stock DiscordRPC_SetAsset(playerid, const asset[], const assetText[], bool:isLarge, bool:updateImmediately = true)
{
    new BitStream:bs = BS_New();

    BS_WriteBool(bs, updateImmediately);
    BS_WriteString32(bs, asset);
    BS_WriteString32(bs, assetText);

    new ret = PR_SendRPC(bs, playerid, isLarge ? SET_PRESENCE_ASSET_LARGE_DATA : SET_PRESENCE_ASSET_SMALL_DATA);

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

    BS_WriteBool(bs, updateImmediately);
    BS_WriteString32(bs, stateText);

    new ret = PR_SendRPC(bs, playerid, SET_PRESENCE_STATE);

    BS_Delete(bs);
    return ret;
}

stock DiscordRPC_SetDetails(playerid, const details[], bool:updateImmediately = false)
{
    new BitStream:bs = BS_New();

    BS_WriteBool(bs, updateImmediately);
    BS_WriteString32(bs, details);

    new ret = PR_SendRPC(bs, playerid, SET_PRESENCE_DETAILS);

    BS_Delete(bs);
    return ret;
}

stock DiscordRPC_SetButtons(playerid, index, const button[], const buttonUrl[], bool:updateImmediately = false)
{
    new BitStream:bs = BS_New();

    BS_WriteBool(bs, updateImmediately);
    BS_WriteUint8(bs, index);
    BS_WriteString32(bs, button);
    BS_WriteString32(bs, buttonUrl);

    new ret = PR_SendRPC(bs, playerid, SET_PRESENCE_BUTTONS);

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
    new ret = PR_SendRPC(bs, playerid, FORCE_UPDATE_PRESENCE);
    BS_Delete(bs);
    return ret;
}

stock DiscordRPC_InitData(playerid, const applicationId[], const stateText[], const details[], const largeAsset[] = "", const largeAssetText[] = "", const smallAsset[] = "", const smallAssetText[] = "", const button1[] = "", const button1Url[] = "", const button2[] = "", const button2Url[] = "")
{
    new BitStream:bs = BS_New();
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

    new ret = PR_SendRPC(bs, playerid, INIT_PRESENCE);

    BS_Delete(bs);
    return ret;
}