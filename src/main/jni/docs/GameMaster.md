# Game Master (GM) Menu Feature

## Description
Enables the internal developer/debug menu used by game designers and testers.

## Implementation Details
- **Hooking `TestGM::Update`**: Intercepts the update loop of the `TestGM` class.
- **Memory Manipulation**: 
    - Sets the `isThread` field to `2` (Force Enable).
    - Sets the `debugMoreInfo` field to `true`.
- **Optimization**: Field offsets are cached using `Il2CppGetFieldOffset` on the first run to ensure maximum performance in the update loop.

## Configuration
Controlled by the `EnableDebugMenu` key in `config.json`.
