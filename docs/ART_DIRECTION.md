# Art Direction — The Last Side Quest

## Goal

The game should look like a polished small indie fantasy RPG, not a browser prototype and not a generic Unreal tech demo.

The joke works because the presentation treats the missing-cat quest with absurd sincerity. The world should be attractive enough that the final reveal of a normal orange house cat under a colossal boss title lands harder.

## Style

**Stylized realism.** Believable materials, lighting, scale, foliage, weathering, and atmosphere, with slightly exaggerated shapes and color separation for readability.

Avoid:
- untextured primitives in final screenshots
- flat evenly lit environments
- neon fantasy clutter everywhere
- random asset-pack styles mixed together
- huge empty terrain
- excessive bloom
- muddy gray/brown scenes with no focal hierarchy

## Area identities

### Village — warm aftermath
- golden late-afternoon / early-evening light
- warm plaster, timber, stone, flowers, carts, banners and celebration leftovers
- soft foliage movement and cozy smoke/chimney ambience
- kingdom is safe; this should feel comforting and mildly ridiculous

### Forest — storybook unease
- cooler greens and blue-green shadows
- shafts of light through canopy, localized fog, dense undergrowth around a narrow readable path
- signs that Mittens passed through should become increasingly ominous
- never make the playable route hard to read

### Ruins — disproportionate dread
- monumental stone architecture compared with the trivial quest
- colder moonlight/ambient fill contrasted with warm torches
- decals, moss, grime, broken masonry and dust motes
- stronger contrast and deeper shadow than the forest

### Boss chamber — maximum theatrical nonsense
- cathedral-scale composition
- throne centered as a strong silhouette
- dramatic backlight/rim light around a tiny orange cat
- fire, embers, fog and controlled cinematic contrast
- the room should visually scream FINAL BOSS before the title appears

## Unreal rendering target

Desktop high-quality target:
- Lumen global illumination/reflections
- Virtual Shadow Maps
- Nanite for appropriate environment meshes
- high-quality PBR textures/material instances
- volumetric fog used deliberately
- Niagara for particles/embers/dust
- decals for environmental breakup
- Post Process Volume with restrained color grading, bloom and exposure
- cinematic camera work for quest/boss reveals where useful

## Characters and animation

Prefer a high-quality third-person humanoid character rig with clean locomotion, attack, hit and death animations. NPCs can reuse compatible rigs/animation sets. Avoid a visual mismatch where the environment is detailed but characters look like primitive mannequins in the final presentation.

Mittens should look like an ordinary believable orange domestic cat. The humor depends on the visual mismatch between his normal appearance and his apocalyptic presentation.

## Performance rule

This is a tiny game, so use the small scope to spend visual budget aggressively. Still maintain a stable playable frame rate on a reasonable modern gaming PC; visual features that cause instability should be scaled before core gameplay is compromised.
