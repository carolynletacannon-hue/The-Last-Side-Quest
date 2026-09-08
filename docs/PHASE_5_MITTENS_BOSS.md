# Phase 5 — Mittens, Devourer of Worlds

## What is implemented

`AMittensBoss` is the one C++ boss. It owns the chamber entrance trigger and temporary entrance blocker, target tracking, three attacks, 300-point health, defeat, and pickup. `APhase4World` extends the route into a compact 2,200 × 2,800 cm arena with a reliable collision floor, invisible side/back walls, warm lights, navigation coverage, and non-colliding art slots. The source HUD draws the boss title and live health fraction while the encounter is active. No Level Blueprint is required.

## Exact quest progression

1. The lever advances `OpenRuinsGate → ReachMittens`.
2. Crossing the boss-owned entrance trigger once advances `ReachMittens → FightMittens`, closes the entrance, starts presentation/music hooks, and enables combat.
3. Walking into the chamber cannot advance farther. Mittens cannot be interacted with while alive.
4. Zero health stops combat, reopens the entrance, shows **Mittens: “Meow.”**, and advances `FightMittens → PickUpMittens`.
5. `[E] Pick up Mittens` advances `PickUpMittens → ReturnToMildred` and hides/disables the world cat.
6. Phase 5 stops there. The Mildred ending remains Phase 6.

The former generic `ReachMittens → PickUpMittens` threshold no longer exists.

## Default behavior and tuning

| Setting | Default |
|---|---:|
| Max health | 300 (three times the 100-point normal-enemy default) |
| Move speed | 380 cm/s |
| Shared cooldown between completed attacks | 2.25 s |
| Scratch range | 175 cm |
| Attack contact radius | 135 cm |
| Pounce distance cap | 900 cm |
| Pounce wind-up / travel / recovery | 0.75 / 0.32 / 1.0 s |
| Pounce damage | 18 |
| Scratch wind-up / beats / recovery | 0.45 s / 3 at 0.22 s / 0.8 s |
| Scratch damage | 6 per beat (18 maximum) |
| Chaos wind-up / travel / recovery | 0.6 / 0.55 / 0.8 s |
| Chaos damage | 12, once per dash |
| Arena radius | 1,100 cm |

At close range Mittens scratches. At range he randomly pounces toward the player or dashes toward the opposite side of the arena. Pounce and dash each have a one-hit latch, so overlap cannot apply damage every frame. All attack movement is swept and clamped to the arena circle. Recovery and the common cooldown prevent instant repeats. The optional Meow is presentation-only at encounter start, avoiding another damage system.

These are forgiving source defaults, not verified final balance. Tune only after a full PIE run with final cat scale and animations; aim for a readable 1.5–3 minute first attempt rather than changing player balance.

## Create and configure `BP_MittensBoss`

1. Compile C++, then create `/Game/Boss/BP_MittensBoss` as a Blueprint child of **MittensBoss**.
2. Select inherited **Mesh** and assign a licensed skeletal mesh of a small, pristine, extremely fluffy white domestic house cat wearing a pink jeweled collar. Fit it to the inherited capsule (34 cm radius, 36 cm half-height). Do not use a giant, armored, demonic, glowing, or mutated cat.
   Hide inherited **Placeholder Cat Body** after the real mesh is visible; the stretched Engine sphere is only an obvious source-build proxy.
3. Assign its Animation Blueprint. Useful clips are **idle**, **walk/run**, **pounce/jump**, **scratch**, **hit reaction**, and **sit/rest**. Root motion is not required; C++ moves attacks.
4. In `BP_Phase4World` Class Defaults, set **Phase 5 | Boss → Mittens Boss Class** to this child.
5. Implement only cosmetic Blueprint events: encounter stinger, boss music start/stop, pounce telegraph/impact, scratch beat, chaos dash, meow, hit, defeat, and pickup. Events may play montages, sounds, Niagara, camera shake, or material pulses and may safely remain empty. Keep defeat in a sit/rest pose, not a death pose.
6. The inherited **Boss Entrance Visual** is an optional gate presentation. Assign a suitable mesh if desired; its sibling box remains authoritative collision.

## Assign boss-room decorative meshes

On `BP_Phase4World`, fill **Phase 5 | Boss Chamber Art** with a cohesive licensed ruins kit: floor, monumental columns, arches, braziers, rubble, and one throne. Generated art has collision disabled. Never delete the broad route floor, three invisible chamber boundary boxes, boss entrance trigger, or entrance blocker. Keep Mittens at/in front of the far throne, preserve the central axis, and keep the 1,100 cm combat circle free of decorative collision. Background arches may imply a much larger cathedral without enlarging play space. Use cool ambient fill/fog and warm fires; add Niagara flames/embers and sound in editor or Blueprint later.

## Placeholder versus production-ready

Production-ready in source: quest ordering, health-driven HUD, attacks/damage latches, defeat/pickup, restart-by-level-reload behavior, arena clamp, collision floor/blockers, and empty presentation hooks. Placeholder/manual: Engine basic-shape route/chamber geometry, visible cat presentation when no child is assigned, throne/architectural art, animations, VFX, audio, boss music, final exposure/fog, cinematic camera work, and empirical balance. No third-party cat or environment license is claimed.

## Exact Play-In-Editor checklist

1. Start a clean PIE session; confirm no boss bar and the entrance is open.
2. Complete Mildred, guard, goblin, ruins entrance, inscription (optional), lever, and skeleton route.
3. Before `ReachMittens`, approach/cross the chamber and verify no encounter or quest skip.
4. At `ReachMittens`, cross once: verify `FightMittens`, title/health bar, start hooks, and closed gate.
5. Re-contact the trigger and verify no duplicate encounter, music, or boss.
6. Verify camera look/movement remain normal after the lightweight presentation hook.
7. Observe pounce wind-up, contact damage, impact hook, and recovery; sidestep it without damage.
8. Approach and observe three scratch beats; retreat outside range and verify no damage through walls.
9. Observe chaos dash, at most one damage event, arena containment, and return to normal behavior.
10. Attack Mittens; verify actual health drives the bar and the hit hook fires.
11. Pull Mittens toward every edge/throne; verify he cannot leave the floor, pass walls permanently, or continue attacking through the closed entrance.
12. Reach zero health: verify attacks stop, bar hides, entrance opens, cat rests, quest becomes `PickUpMittens`, and **Mittens: “Meow.”** appears.
13. Advance the meow, approach, verify `[E] Pick up Mittens`, pick up once, and confirm `ReturnToMildred` plus hidden/disabled world cat.
14. Separately die during each attack, press R/Menu, and confirm a fresh living Mittens, open entrance, absent boss UI, reset quest, and startable encounter.

## Common failure cases

- **Pickup appears on entry:** remove any legacy `QuestProgressTrigger` configured `ReachMittens → PickUpMittens`; only the boss may own these transitions.
- **Fight does not start:** inspect current quest step, `MittensBossClass`, boss position, inherited entrance-trigger collision, and whether more than one `Phase4World` exists.
- **No pickup prompt:** finish the defeat dialogue, check `PickUpMittens`, boss capsule Pawn collision, and the player's 225 cm interaction radius.
- **Boss bar sticks:** verify the inherited Health Component reached defeat and that restart reloads the map rather than respawning only the pawn.
- **Mittens escapes/sticks:** restore invisible boundaries, remove decorative collision, check floor/navigation coverage, and retain the 1,100 cm clamp.
- **Gate traps the player:** inspect inherited Boss Entrance Blocker; C++ disables it at defeat. Do not replace its collision with a decorative mesh.
- **Damage feels unavoidable:** check cat/capsule scale and animation timing before changing wind-ups, contact radii, or recovery windows.
- **No sound/VFX:** cosmetic events intentionally do nothing until implemented in `BP_MittensBoss`.

Unreal Engine 5.8 and licensed production assets are required for compilation, PIE, visual review, animation review, and real balance validation. Source/static inspection is not a substitute.
