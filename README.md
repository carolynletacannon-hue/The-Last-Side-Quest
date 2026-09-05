# The Last Side Quest

A tiny comedic browser-based 3D RPG made in one day.

You are a legendary adventurer who has already defeated the Dark Lord and saved the kingdom. Before leaving town, you notice one unfinished quest: Mildred's missing orange cat, Mittens.

What should be a five-minute errand escalates from a peaceful village to a forest, ancient ruins, and a final confrontation with **Mittens, Devourer of Worlds**.

## Goal

Ship a complete 10–15 minute playable RPG with a beginning, middle, boss fight, ending, and credits.

## Core loop

Village → Forest → Ruins → Boss Room → Return to Village → Credits

## Tech stack

- React 19
- TypeScript
- Vite
- Three.js
- React Three Fiber
- @react-three/drei
- Zustand

This intentionally mirrors the technical approach used by Stonebound so development can move quickly.

## Strict scope

Included:
- Third-person movement and camera
- One basic melee attack
- Player health
- Simple enemy AI and health
- Interaction prompts
- Linear dialogue
- One quest objective at a time
- One village, one forest, one ruin, one boss room
- 2–3 regular enemy types
- Mittens boss fight
- Ending and credits

Not included:
- Inventory grid
- Crafting
- Skill trees
- Leveling
- Equipment system
- Procedural generation
- Open world systems
- Branching dialogue
- Multiplayer
- Save system unless trivial
- Anything else that threatens finishing the game

## Story beats

1. Mildred asks the player to find Mittens. Reward: 3 gold.
2. Town guard reports an orange cat entering the forest.
3. Forest enemies and environmental clues imply Mittens is terrifying.
4. A surviving goblin calls him "THE BEAST."
5. The player enters ancient ruins marked with warnings about "The Devourer."
6. The final chamber reveals a tiny orange house cat on a throne.
7. Boss title: **MITTENS — DEVOURER OF WORLDS**.
8. After defeat, Mittens simply meows and allows himself to be picked up.
9. Mildred rewards the player 3 gold and casually explains that Mittens gets grumpy when hungry.
10. Mildred asks whether the player can find her other cat. Player: "No." Cut to black.

## Development rule

Every major task should leave the game playable. Prefer ugly-but-working placeholders over unfinished polished systems.

See `docs/GAME_DESIGN.md`, `docs/ROADMAP.md`, and `AGENTS.md` before making large changes.
