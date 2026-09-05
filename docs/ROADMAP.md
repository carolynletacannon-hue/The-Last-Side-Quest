# Roadmap — Finish the Game

## Phase 1 — Project boots
- Install dependencies
- Vite + React + R3F canvas renders
- Basic placeholder environment
- Build succeeds

## Phase 2 — Player controller
- Third-person movement
- Camera follow/orbit
- Grounding and collision good enough for small levels
- Interaction key reserved

## Phase 3 — Combat
- Sword attack
- Damage and hit feedback
- Player health
- Simple death/restart

## Phase 4 — Generic enemies
- Reusable health/damage behavior
- Chase and melee attack
- Slime/goblin/skeleton variants via stats and simple visuals

## Phase 5 — Interaction and dialogue
- Look/range based interact prompt
- Linear dialogue box
- NPC dialogue data
- Input does not fight with movement/combat

## Phase 6 — Quest state
- Current objective HUD
- Quest stages advance only from intended triggers
- Mildred → Guard → Forest → Goblin → Ruins → Mittens → Mildred

## Phase 7 — Village
- Small readable village
- Mildred, guard, optional villager
- Start quest and final turn-in

## Phase 8 — Forest
- Short path
- 4–6 enemies
- Environmental clues
- Goblin survivor
- Entrance to ruins

## Phase 9 — Ruins
- Small dungeon
- 2–3 skeletons
- One lever/gate interaction
- Boss door and ominous text

## Phase 10 — Mittens boss
- Tiny orange cat visual
- Boss reveal/title card
- Boss health bar
- Pounce, scratch, dash
- Defeat state and pickup interaction

## Phase 11 — Ending
- Return to Mildred
- 3 Gold reward joke
- Final “other cat” exchange
- Credits

## Phase 12 — Polish and ship
- Replace only the most distracting placeholders
- Audio where easy
- Tune combat difficulty
- Fix progression blockers
- Build and deploy

## Scope triage
If time is running short, cut in this order:
1. Optional villager
2. Slime enemy type
3. Lever puzzle (door can open automatically)
4. Meow boss attack
5. Fancy animations
6. Audio

Never cut: Mildred, quest progression, forest, ruins, Mittens reveal/fight, 3 Gold ending, credits.
