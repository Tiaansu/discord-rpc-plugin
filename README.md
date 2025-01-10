# Discord RPC

It is a server-client plugin that controls discord rpc using [Pawn.RakNet](https://github.com/katursis/Pawn.RakNet).

## API
- DiscordRPC_SetApplicationID - sets application id of discord rpc
- DiscordRPC_SetAsset - set the large/small asset of discord rpc
    - DiscordRPC_SetLargeAsset - ^
    - DiscordRPC_SetSmallAsset - ^
- DiscordRPC_SetState - set the state text of discord rpc
- DiscordRPC_SetDetails - set the details text of discord rpc
- DiscordRPC_SetButtons - set the button 1/2 of discord rpc
    - DiscordRPC_SetButton1 - ^
    - DiscordRPC_SetButton2 - ^
- DiscordRPC_ForceUpdate - forces the update of discord rpc (should not be used very frequently)
- DiscordRPC_InitData - inits the data of discord rpc (should be used within `OnPlayerRequestDiscordRichPresence`/`OnPlayerRequestDiscordRPC`)

## Supported version
- 0.3.7 R1
- 0.3.7 R3
- 0.3.7 R5
- 0.3.7-DL / 0.3.8