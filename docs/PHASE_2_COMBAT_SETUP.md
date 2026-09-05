# Phase 2 Combat Setup

Phase 2 is playable from C++ without input or UI assets. The player has a forward sword sweep, health, damage flash, death screen, and restart input. `ASideQuestEnemy` supplies shared health, sight, chase, melee attack, hit/death events, and tunable stats for every normal enemy.

## Controls

- **Left mouse / right trigger:** sword attack (0.55-second cooldown)
- **R / gamepad Menu:** restart the current level after death
- Phase 1 movement, camera, and jump controls are unchanged.

## Required editor setup

Enemy navigation is the only required level setup:

1. Open the test level.
2. In the **Place Actors** panel, search for **Nav Mesh Bounds Volume**.
3. Drag it into the level.
4. With the volume selected, use **R** and the transform fields in **Details > Transform** to cover every floor area where enemies should chase the player.
5. Press **P** to visualize navigation. Confirm the playable floor around each enemy is green, then press **P** again to hide it.
6. Drag one or more **Side Quest Enemy** actors from **Place Actors > All Classes** into the green area.
7. Ensure the level has a **Player Start**, then click **Play**.

No Blueprint is required. A source-only enemy uses the inherited character capsule and is functionally testable, but its mesh is intentionally an asset hook rather than final art.

## Recommended presentation setup

Create thin Blueprint children rather than new gameplay classes:

1. In the Content Browser, right-click and select **Blueprint Class**.
2. Expand **All Classes**, search for **SideQuestEnemy**, select it, and name the child `BP_Enemy_Goblin`.
3. Open it and select **Mesh (Inherited)**. Assign a licensed goblin skeletal mesh and its Animation Blueprint. Adjust its relative transform to fit the inherited capsule.
4. Select **Health Component (Inherited)** and set **Max Health**. In **Class Defaults**, tune **Attack Damage**, **Attack Range**, **Attack Cooldown**, **Target Memory Duration**, and Character Movement's **Max Walk Speed**.
5. In the Event Graph, optionally implement **Play Attack Presentation**, **Play Hit Presentation**, and **Play Death Presentation** with animation, sound, Niagara, or brief material feedback. These events are cosmetic; damage and death remain in C++.
6. Duplicate the Blueprint as `BP_Enemy_Slime` and `BP_Enemy_Skeleton`, then change only mesh/animation, capsule fit, movement speed, health, and attack values. Do not create separate AI graphs.
7. For the player Blueprint, select **Attack Montage** in **Class Defaults** and assign a sword-swing montage. Implement **Play Attack Hit Presentation** for impact VFX/audio, **Play Hit Presentation** for a hit reaction, and **Play Death Presentation** for the death animation.

The C++ sweep is applied immediately when attack input is accepted for responsive placeholder play. Choose an attack montage whose impact pose begins promptly. A later presentation pass may align cosmetic timing without moving authoritative damage into Blueprint.

## Suggested normal-enemy tuning

| Variant | Max Health | Damage | Cooldown | Walk speed | Visual role |
| --- | ---: | ---: | ---: | ---: | --- |
| Goblin | 68 | 14 | 1.1 s | 330 | quick, small humanoid |
| Slime | 45 | 10 | 1.4 s | 220 | slow, soft silhouette |
| Skeleton | 100 | 20 | 1.5 s | 280 | sturdy, readable melee threat |

These are Blueprint-default suggestions, not separate mechanics. They preserve one shared enemy implementation.

## Phase 2 verification

1. Place a Nav Mesh Bounds Volume, a Player Start, and one Side Quest Enemy on a walkable floor.
2. Play and approach the enemy from the front. It should detect and chase the player.
3. Let it enter melee range. The lower-left health bar should decrease and the screen should briefly flash red on every hit.
4. Face the enemy and press left mouse three times, waiting briefly between clicks. With default values, the third accepted strike kills it; repeated clicks inside the cooldown should not add damage.
5. Let another enemy reduce player health to zero. Movement and attacking should stop and the death overlay should appear.
6. Press **R**. The current level should reload at its Player Start.
7. Repeat with goblin, slime, and skeleton Blueprint children to confirm the same detection/chase/attack/death behavior uses their different configured stats and visuals.
