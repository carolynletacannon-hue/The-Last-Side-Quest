# Asset Plan

Do not build final art from primitives. Use placeholders only until the matching production asset is available.

## Priority asset categories

1. **Player character + animations**
   - polished third-person fantasy/adventurer character
   - idle/walk/run/turn/jump
   - sword attacks, hit reactions, death

2. **Medieval village kit**
   - timber/plaster houses
   - stone paths, fences, carts, barrels, signs
   - celebration props/banners

3. **Forest environment**
   - Nanite-capable trees/rocks where appropriate
   - grass, shrubs, ferns, fallen logs
   - ground materials and decals

4. **Ancient ruins/dungeon kit**
   - monumental stone walls/arches/pillars
   - stairs, doors, throne, rubble
   - torch/fire props

5. **Enemies**
   - goblin or equivalent small humanoid
   - slime/creature
   - skeleton
   - compatible attack/death animations
   - Phase 2 exposes all three as presentation/stat variants of `ASideQuestEnemy`; implement its attack, hit, and death presentation events in thin Blueprint children

6. **Mittens**
   - believable orange domestic cat model
   - idle, walk/run, jump/pounce and attack-capable animations if possible
   - Phase 5 uses `AMittensBoss` / `BP_MittensBoss`; add hit reaction and sit/rest animations, and
     keep damage, quest progression, and attack timing in C++

7. **VFX/audio**
   - sword impacts
   - hit sparks/dust
   - torch/fire/embers
   - ambient village/forest/ruins beds
   - disproportionately epic boss music

## Phase 2 presentation hooks

- The player Blueprint can assign a sword attack montage and implement attack-impact, hit, and death presentation events.
- Goblin, slime, and skeleton Blueprint children share one C++ enemy and expose mesh/animation plus attack, hit, and death presentation events.
- No third-party combat assets are committed. Record source and license details here when the final character, animation, sound, or VFX assets are selected.

## Phase 3 presentation hooks

- `AQuestInteractableActor` Blueprint children provide meshes, animation/audio reactions, labels, and
  the authoritative dialogue from `DIALOGUE.md`; C++ owns interaction and quest progression.
- Mildred, the guard, and the goblin may share a compatible NPC rig. The inscription, lever, and
  Mittens pickup need only suitable static/skeletal presentation assets in their final areas.
- No third-party NPC, prop, UI, or audio assets are committed in Phase 3. Record their source and
  license here when selected.

## Phase 4 world hooks

`APhase4World` is the source-controlled playable layout and fallback presentation. It intentionally
uses Engine basic-shape meshes only when no project art is assigned, so a clean clone remains
playable; those gray shapes are **not final art** and must not be used for release screenshots.

Create `BP_Phase4World` and assign one cohesive set of licensed assets to its exposed categories:

- **Route:** retain `Ground Collision Mesh`; assign warm village, cool forest, and damp/mossy ruins
  material instances. The generated floor owns dependable collision independently of decoration.
- **Village:** building and small-prop arrays.
- **Forest:** tree, rock, and undergrowth arrays.
- **Ruins:** wall, column/arch, rubble, and gate visual slots.
- **Story:** dead goblin, scratched tree, and damaged cart slots.
- **Cast:** Phase 3 presentation children for Mildred, guard, survivor, inscription, and lever.
- **Enemies:** the Phase 2 goblin, slime, and skeleton presentation children. The authored counts are
  three goblins + two slimes in the forest and three skeletons in the ruins.

Free acquisition categories to search (verify the current product page, compatibility, and license
before adding it; no specific current Fab listing was verifiable during Phase 4.5):

1. Quixel Megascans medieval/European surfaces, forest rocks, trees, moss, and ruined masonry.
2. Epic-authored free environment collections on Fab for compatible village modular pieces.
3. A single animation-compatible free character family for NPCs/goblins/skeletons.

Nothing from Fab or Megascans is committed by Phase 4. After selecting exact products, add their
names, store URLs, license terms, installed project paths, and uses to this file before committing
their references.

The beginner workflow, placement coordinates, lighting ranges, performance guidance, and visual
acceptance checklist are in `PHASE_4_5_VISUAL_PASS.md`. The expected hands-on editor pass is roughly
12–20 hours after asset download, plus 4–8 hours if screenshot-level polish is required.

## Phase 5 boss-room hooks

`APhase4World` exposes non-colliding boss floor, monumental column, arch, brazier, rubble, and throne
slots plus a `MittensBossClass`. Invisible generated floor and boundary boxes remain authoritative.
Use an ordinary, realistically proportioned orange domestic cat—not armor, mutation, glow, or a
monster silhouette. No cat, animation, sound, music, Niagara, or licensed chamber asset is committed
in Phase 5; record the exact source and license here before adding one.

## Source policy

Prefer Epic Fab / Unreal Marketplace compatible assets with clear licenses. Megascans may be useful for rocks, surfaces and environmental detail when style-consistent. Do not commit third-party paid/licensed source assets to Git unless their terms explicitly allow repository distribution.

For each external asset actually used, record:
- asset name
- source/store
- license notes
- where it is used
- whether it must be installed separately after cloning

## Consistency rule

A smaller number of visually compatible packs is better than mixing many unrelated packs. Match texel density, material response, proportions, color palette and character style before adding more detail.
