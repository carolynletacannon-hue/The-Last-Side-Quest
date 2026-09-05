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

6. **Mittens**
   - believable orange domestic cat model
   - idle, walk/run, jump/pounce and attack-capable animations if possible

7. **VFX/audio**
   - sword impacts
   - hit sparks/dust
   - torch/fire/embers
   - ambient village/forest/ruins beds
   - disproportionately epic boss music

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
