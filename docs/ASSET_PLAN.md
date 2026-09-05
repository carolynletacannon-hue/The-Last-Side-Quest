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

- **Art:** a modular walkable ground/road piece for `Ground Mesh`, a compatible tree/column/vertical
  dressing piece for `Prop Mesh`, and warm village, cool forest, and damp/mossy ruins materials.
- **Cast:** Phase 3 presentation children for Mildred, guard, survivor, inscription, and lever.
- **Enemies:** the Phase 2 goblin, slime, and skeleton presentation children. The authored counts are
  three goblins + two slimes in the forest and three skeletons in the ruins.

Recommended free acquisition shortlist (verify the current Fab license before adding it):

1. Quixel Megascans medieval/European surfaces, forest rocks, trees, moss, and ruined masonry.
2. Epic-authored free environment collections on Fab for compatible village modular pieces.
3. A single animation-compatible free character family for NPCs/goblins/skeletons.

Nothing from Fab or Megascans is committed by Phase 4. After selecting exact products, add their
names, store URLs, license terms, installed project paths, and uses to this file before committing
their references.

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
