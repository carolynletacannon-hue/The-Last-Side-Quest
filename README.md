# The Last Side Quest

A tiny comedic third-person fantasy RPG built in **Unreal Engine 5.8**.

You already killed the Dark Lord. You already saved the kingdom. Unfortunately, Mildred still needs you to find her cat.

## Visual target

This project prioritizes a polished indie-fantasy presentation over browser portability. Target features include:

- Unreal Engine 5.8
- Third-person C++ gameplay foundation
- Lumen global illumination and reflections
- Nanite-ready environment assets
- Virtual Shadow Maps
- Volumetric fog and atmospheric lighting
- High-quality PBR materials
- Cinematic post-processing
- Stylized-realistic medieval fantasy art direction
- Strong contrast between the cozy village and absurdly dramatic Mittens boss arena

## Scope

The game should take roughly 10–15 minutes:

Village → Forest → Ruins → Mittens boss → Return to Mildred → Credits

The game contains one linear quest, simple melee combat, a handful of enemies and NPCs, and one ridiculous boss fight.

## Development rule

**Finished beats enormous.** Do not add inventory, crafting, skill trees, leveling, equipment systems, open-world systems, multiplayer, procedural quest generation, or unrelated RPG infrastructure.

## Engine

Use Unreal Engine **5.8**. Gameplay systems should prefer C++ and data-driven Unreal patterns so Codex can work effectively from the repository. Blueprints may be used as thin presentation/asset layers where they materially improve iteration, but core game logic should not exist only inside opaque Blueprint graphs.

Read `AGENTS.md`, `docs/GAME_DESIGN.md`, `docs/ART_DIRECTION.md`, and `docs/CODEX_START_HERE.md` before implementing gameplay.
