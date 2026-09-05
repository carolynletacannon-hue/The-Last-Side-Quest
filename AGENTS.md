# Codex Instructions — The Last Side Quest

This repository is a deliberately tiny Unreal Engine 5.8 third-person fantasy RPG.

## Prime directive

Build a complete 10–15 minute game. Do not turn this into a reusable RPG framework.

## Engine and architecture

- Unreal Engine 5.8.
- Prefer C++ for gameplay logic that Codex must maintain.
- Use standard Unreal systems rather than custom infrastructure when possible.
- Keep Blueprints thin: presentation, asset references, simple tuning, animation hookup, and level dressing are acceptable.
- Avoid putting critical quest/combat logic only in Blueprint graphs.
- Use Enhanced Input.
- Use Character/CharacterMovement for the player unless a simpler built-in path is clearly better.
- Use UMG for HUD/dialogue.
- Use Data Assets/Data Tables only when they reduce duplicated content; do not build elaborate data frameworks.

## Visual target

This project is intentionally graphics-forward.

- Lumen GI/reflections for desktop target.
- Nanite-ready static environment assets where useful.
- Virtual Shadow Maps.
- PBR materials with believable roughness/normal response.
- Volumetric fog, atmospheric lighting, decals, foliage, particles, and post-processing should support the mood.
- Art direction is stylized-realistic medieval fantasy, not blocky low-poly prototype art.
- Placeholder primitives are allowed during implementation, but each phase should leave clear asset hooks for high-quality replacements.
- Never sacrifice readability or completion for visual effects.

## Strict scope

Game route: Village → Forest → Ruins → Mittens boss → Village ending.

Allowed core systems:
- third-person movement/camera
- melee attack
- health/damage/death
- 2–3 reusable enemy variants
- interaction prompts
- linear dialogue
- linear quest state
- boss fight
- ending/credits

Do NOT add inventory, crafting, equipment, loot tables, leveling, skill trees, open-world systems, procedural quests, multiplayer, save-slot UI, shops, factions, reputation, or generalized RPG architecture.

## Workflow

1. Read the current GitHub issue and relevant docs.
2. Implement only that phase and required dependencies.
3. Keep code small and readable.
4. Compile/test when Unreal tooling is available.
5. Do not begin the next issue automatically.
6. Document any editor-only setup that cannot be represented in source control.

## Asset policy

Prefer high-quality licensed/free Unreal-compatible assets over hand-modeling everything. Do not commit Marketplace/Fab assets to this repository unless their license and source-control workflow permit it. Keep a record of required assets and their intended use in `docs/ASSET_PLAN.md`.
