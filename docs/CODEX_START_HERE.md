# Codex Start Here

Read these files before changing code:

1. `AGENTS.md`
2. `README.md`
3. `docs/GAME_DESIGN.md`
4. `docs/ART_DIRECTION.md`
5. `docs/ASSET_PLAN.md`
6. `docs/DIALOGUE.md`
7. the current GitHub issue

This project now targets **Unreal Engine 5.8**, not React/Three.js.

## First task

Start with GitHub Issue #1 only: player movement and third-person camera.

Before implementing:

- inspect the `.uproject`, `Config`, and `Source` structure
- verify the Unreal 5.8 C++ project layout is valid
- prefer the standard Unreal Third Person approach using `ACharacter`, `CharacterMovement`, SpringArm/Camera, and Enhanced Input
- expose asset references/tuning cleanly for later Blueprint/editor hookup
- do not begin combat, quest, dialogue, or level-building work yet

## Visual requirement

Even at prototype stage, structure the player/camera and rendering setup so the project can use the visual target in `ART_DIRECTION.md`: Lumen, Nanite-ready environment assets, Virtual Shadow Maps, volumetrics, PBR materials, and cinematic post-processing.

Placeholders are fine for functionality. Do not mistake placeholders for the final art plan.

## Finish condition

When the phase is complete:

- compile/test with Unreal tooling if available
- report exactly what changed
- list any Unreal Editor steps the user must perform because they cannot be represented as text/source assets
- stop and do not start Issue #2
