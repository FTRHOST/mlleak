# Unlock Skin Feature

## Description
This feature allows the player to use any skin in the game locally. It does not actually purchase the skins on the server.

## Implementation Details
- **Hooking `SystemData::GetHeroSkin`**: When the game checks if a player owns a skin, the hook intercepts the call and returns a fake `CmdHeroSkin` instance if the real one is missing.
- **Antiban Measures**: 
    - Hooks `GameServerConfig::SendRawData`.
    - Blocks specific packet IDs related to skin synchronization and verification to prevent the server from detecting local skin modifications.
    - Blocked Message IDs include: `1015, 1016, 1019, 1020, 1031, 1032, 1035, 1036, 1160, 1161, 1162, 1163, 1208, 10017, 10018, 10192, 10193, 10603, 10604, 19457, 19458, 19459, 19460`.

## Configuration
Controlled by the `UnlockCustomSkin` key in `config.json`.
