# Roadmap

This game is now built in **Unreal Engine 5.8** with a C++ gameplay foundation and a graphics-first presentation target.

## Phase 1 — Player foundation
- Unreal project opens cleanly
- Third-person `ACharacter`
- CharacterMovement locomotion
- Spring Arm + camera
- Enhanced Input mapping
- clean asset/animation hooks

## Phase 2 — Combat and enemies
- sword attack
- health/damage/death
- simple reusable enemy actor/character base
- chase/melee AI
- lightweight variants for goblin, slime/creature, and skeleton
- animation/VFX hooks

## Phase 3 — Interaction, dialogue, quest
- interact trace/range prompt
- UMG dialogue box
- linear dialogue data
- objective HUD
- explicit quest-state progression

## Phase 4 — Full playable route
- village
- forest
- ruins
- boss approach/chamber
- navigation/collision/lighting pass
- production-asset replacement begins

## Phase 5 — Mittens, Devourer of Worlds
- dramatic reveal/title card
- pounce
- scratch combo
- chaos dash
- boss health bar
- defeat/interactable state
- cinematic lighting/VFX/audio hooks

## Phase 6 — Ending and ship
- return to Mildred
- 3 Gold reward gag
- other-cat joke
- credits
- progression-blocker fixes
- final lighting/material/foliage/VFX/audio pass
- packaged Windows build

## Visual progression rule

Gameplay placeholders are allowed early, but each completed phase should improve presentation rather than leaving every visual decision until the end. Follow `ART_DIRECTION.md` and `ASSET_PLAN.md`.

## Definition of done

A player can launch the packaged game and play from Mildred's first line through the credits without editor intervention.
