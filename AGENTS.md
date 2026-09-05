# Codex Instructions — The Last Side Quest

## Mission
Build and ship a complete, tiny, comedic 3D RPG. Completion matters more than architectural perfection.

## Non-negotiable scope
The game is a 10–15 minute linear experience:
Village → Forest → Ruins → Boss Room → Village → Credits.

Do not add systems that are not required for that path. In particular, do not add inventory, crafting, leveling, equipment, procedural generation, branching dialogue, multiplayer, or generalized RPG frameworks.

## Technical direction
Use the existing React + TypeScript + Vite + Three.js + React Three Fiber + Drei + Zustand stack. Keep components small and understandable. Prefer data-driven constants for dialogue, enemy stats, quest text, and scene tuning.

## Development behavior
- Inspect existing files before changing architecture.
- Make the smallest change that achieves the current task.
- Keep the game runnable after each task.
- Use placeholder geometry/materials when art assets do not yet exist.
- Avoid adding dependencies unless clearly necessary.
- Do not refactor working systems just for style.
- If a task risks expanding scope, choose the simpler implementation.
- Run the build before considering a task complete.

## Gameplay target
- Third-person movement and camera
- Basic sword attack
- Player health and death/restart
- Basic enemy chase/attack behavior
- Simple interaction prompts
- Linear dialogue box
- Quest objective HUD
- Scene progression through one connected world or lightweight area transitions
- Mittens boss with 2–3 readable attacks
- Ending and credits

## Comedy rule
Play the world straight. The joke is the ridiculous escalation of a missing-cat side quest, not constant fourth-wall dialogue.

## Definition of done
A new player can start the game, receive Mildred's quest, follow clues through the forest and ruins, fight Mittens, return him to Mildred, see the 3-gold reward joke, and reach credits without developer intervention.
