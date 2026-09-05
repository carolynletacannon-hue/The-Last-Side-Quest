# Phase 4.5 — Visual production preparation

## Purpose and boundary

This pass prepares the existing Village → Forest → Ruins route for production art. It does **not** make the runtime fallback finished art, change quest ordering, move encounters, or add the Mittens boss. Engine cubes/cylinders remain an emergency fallback so a clean clone is playable.

`APhase4World` now keeps collision, triggers, navigation, interactables, enemies, and gate blocking separate from non-colliding decorative meshes. A `BP_Phase4World` can therefore receive art without transferring gameplay logic to Blueprint. The supplied transforms are composition starting points, not a substitute for looking at and dressing the rendered scene.

## Fastest realistic workflow

Budget **12–20 focused editor hours** for a convincing first pass (about 3–5 village, 4–6 forest, 4–6 ruins, and 1–3 lighting/performance/test hours) after assets download and shaders compile. A screenshot-ready polish pass may take another 4–8 hours. Use one village kit, one ruins kit, and one coherent vegetation/material family; more packs usually create more cleanup, not more quality.

1. Acquire the minimum asset set below.
2. Create one `BP_Phase4World`, fill its category arrays, and verify the whole route once.
3. Add hand-placed dressing around—not instead of—the generated route.
4. Light and color-grade only after all three areas have representative materials.
5. Do three playthroughs: route/readability, collision/navigation, then visual polish.

# A. Assets

## Minimum shopping list

| Priority | Category | Minimum useful contents | Use |
|---|---|---|---|
| 1 | Cohesive medieval village kit | 4–6 house variants, timber/plaster walls, roofs, fence, cart, barrels/crates, signs | Village silhouette and close dressing |
| 1 | Forest biome | 3 tree silhouettes, 3 rocks, fallen log/root, bush, fern, grass, leaf litter | Canopy, blockers, path edge, ground coverage |
| 1 | Modular ancient ruins kit | straight/corner walls, arch, column, stairs, broken variants, rubble | Ruins progression and chamber approach |
| 1 | Ground surfaces | warm dirt, worn stone/cobble, forest soil/leaves, damp ruin stone, moss | Remove the visible gray route floor |
| 2 | Decals/material details | mud, leaves, moss, cracks, dampness, dirt, scratch marks | Break repetition and communicate clues |
| 2 | Lighting/VFX | torch/fire Niagara system, smoke, dust motes, light fog cards if needed | Warm/cool contrast and atmosphere |
| 2 | Characters | compatible hero/NPC, goblin, slime/creature, skeleton models and required animations | Remove gameplay mannequins/placeholders |
| 3 | Audio | village, forest, ruins ambience; fire; wind; subtle stingers | Area identity after visuals are stable |

For foliage, prefer meshes with believable PBR materials, wind support, LODs, and masked-leaf performance. For large static rock/ruin meshes, prefer Nanite-ready assets. Do not require every tiny grass blade to use Nanite.

## Acquisition recommendations and verification rule

Current Fab product availability and license terms could not be reliably verified from this development environment, so this document deliberately does **not** invent a pack name, URL, price, or license. In Fab, filter for **Price: Free**, **Formats: Unreal Engine**, and a supported engine version compatible with 5.8. Favor Epic-published content and coherent Quixel/Megascans collections where the product page explicitly permits use in this Unreal project.

Before adding anything, record the exact product title, seller, product URL, license shown on the listing, supported UE version, installed path, and intended use in `ASSET_PLAN.md`. A “free” price is not itself a license. Avoid custom-code plugins, packs whose demo images are substantially different in style, and packs that require another paid product.

Use this cohesion test before downloading: compare one house, one tree, and one ruin screenshot side-by-side. Reject a combination if scale, saturation, edge sharpness, or material realism makes one look cartoon-flat or photoreal-scanned beside the others. It is easier to slightly stylize realistic materials with color grading than to reconcile incompatible geometry styles.

# B. Import / add to project

## Fab Launcher / Epic Games Launcher route

1. Close Unreal Editor or save everything first.
2. On the product page, confirm **Free**, license, Unreal format, and engine compatibility. Add it to your library.
3. In the Epic Games Launcher **Library / Fab Library**, refresh, find the product, choose **Add to Project**, select *The Last Side Quest*, and choose the nearest supported engine version when the listing allows it. Do not force an incompatible plugin into 5.8.
4. Reopen the project and let shader compilation finish. Do not judge black/checkerboard materials while shaders are compiling.
5. Find the vendor's new top-level folder. Open its overview/demo map first; identify only the meshes and material instances actually needed.
6. Keep the vendor folder intact during evaluation. Renaming or moving assets in the operating-system file browser breaks Unreal references.
7. For assets you will keep, use Content Browser **Asset Actions → Migrate** only when moving between projects. Inside this project, drag assets in Content Browser, choose **Move**, then right-click the old parent folder and **Fix Up Redirectors in Folder**. Do not move shared master materials unless all dependencies move with them.
8. Organize project-owned Blueprint wrappers and material instances under:
   - `/Game/Environment/Village`
   - `/Game/Environment/Forest`
   - `/Game/Environment/Ruins`
   - `/Game/Characters`, `/Game/Enemies`, `/Game/Props`
   - `/Game/Materials`, `/Game/VFX`, `/Game/Audio`
9. Never copy Marketplace/Fab binaries into Git until its license and team distribution terms have been checked. Commit project-authored wrappers and this installation record only when permitted.

## Create the art-authoring Blueprint

1. Compile the C++ project, open Content Browser, and create `/Game/World/Phase4` if absent.
2. Choose **Blueprint Class → All Classes → Phase4World**, name it `BP_Phase4World`.
3. Open **Class Defaults**. Under `Phase 4 | Route (Do Not Remove)`, leave `Ground Collision Mesh` assigned. Set the three ground materials to compatible material instances. This floor is authoritative walkable collision.
4. Under the art categories, add 2–3 variations per array where available. Assign correctly sized production meshes; do not compensate for a centimeter/metre import error with extreme Blueprint scale.
5. Assign the story meshes and gate visual. Decorative generated art has collision disabled by design; the invisible route and gate collision still govern play.
6. Assign the existing cast and enemy Blueprint classes. Compile and save.
7. Place exactly one `BP_Phase4World` at world origin `(0,0,0)` in the gameplay map, deleting a previously placed C++ `Phase4World`. GameMode will not spawn a duplicate.
8. Play once before hand dressing. If a generated mesh is badly oriented or sized, substitute another mesh or hand-place it; do not move the route actor away from origin.

**Reference safety:** duplicate vendor material instances before tuning; never edit a vendor master material casually. Use **Reference Viewer** before deleting anything. Save, source-control, and test after each area rather than after the entire pass.

# C. Village pass (X 0–3200)

1. Stand at player start and frame Mildred first. Put a warm house façade, porch/awning, flowers, two domestic props, and a warm practical light behind or beside her—not between her and the player. Keep a 250–300 cm interaction circle clear.
2. Use the four generated building anchors as the first street silhouettes. Rotate fronts toward the path. Add 2–4 more distant façades or rooflines behind them; they may be inaccessible, but should not reveal missing backs through the camera.
3. Cover the route floor with dirt/cobble material and use irregular stone edges, grass tufts, wheel ruts, and leaf/mud decals to hide its rectangular outline. Do not place a second collision floor that catches feet.
4. Build dense “prop clusters,” not even scatter: cart + sacks + barrel; fence + flowers + sign; crate stack + bucket. Leave calmer negative space around Mildred and the guard so both remain readable.
5. Frame the street with fences and trees beyond the playable edge. Use buildings and foliage to hide the empty horizon and imply streets continuing out of view.
6. Keep the center corridor roughly **400 cm wide**, the line from spawn to Mildred unobstructed, and the approach to the guard/forest exit obvious. Walk it with the camera rotated fully; remove anything that clips the camera.
7. Warmest light and richest small detail belong near Mildred. The forest exit should retain warm rim light but show cooler trees beyond it.

Stop when one screenshot from spawn shows a foreground prop, readable Mildred, midground buildings, a clear road, and layered distant foliage—without showing a cube edge or empty void.

# D. Forest pass (X 3200–8800)

1. Place the largest trees first at alternating sides of the path. Make each bend terminate on a trunk, rock, fallen log, or foliage mass so the player never sees the whole forest corridor at once.
2. Make three reusable clusters: **tree + root + ferns**, **rock + bush + leaf decal**, and **fallen branch + grass**. Duplicate clusters, then rotate 90–220°, swap one mesh variant, and vary scale only about 0.85–1.15. Avoid a regular grid.
3. Add a second and third tree row outside the playable edge. Scale/darken the far row slightly and use fog to lose its bases. These inexpensive layers imply depth; do not expand the walkable footprint.
4. Preserve a 350–450 cm readable trail. Keep foliage off enemy feet and sightlines, and keep tree collision from narrowing combat clearings. Decorative C++ hooks are non-colliding, but hand-placed assets may not be.
5. Break the ground into soil, leaf litter, sparse moss, stones, and roots. Dense undergrowth belongs at edges; the path center gets flattened, warmer/lighter soil and fewer tall plants.
6. Transition gradually: near X 3200 retain warm light and flowers; around X 5500 thicken canopy and cool shadows; after X 7000 add damaged vegetation, stronger fog pockets, and darker ground.
7. At the survivor clearing, make a readable left-to-right story composition:
   - dead goblin near `(7800,-260)` visible before the survivor;
   - three large scratch marks at eye height on the tree near `(7920,430)`;
   - damaged/overturned cart near `(8200,-380)` with a broken trail of cargo;
   - survivor near `(8050,230)`, unobstructed and visually separated from background.
   Use blood/damage sparingly; the escalating absurdity, not gore, carries the scene.
8. In foliage mode, paint one species at a time. Start low density, use random yaw, align grass/ferns to normal, and avoid tree painting until collision settings are understood. Erase a clean route after every density pass.

To catch repetition, stop at three locations, turn backward, and look for matching tree forks or identical rock silhouettes adjacent to one another. Swap or rotate only the obvious repeats.

# E. Ruins pass (X 8800–14000)

1. Check the kit's grid size (often 50 or 100 cm), enable matching translation snap, and assemble one straight bay from wall + column + broken cap. Duplicate bays; use broken pieces and rubble to vary silhouettes.
2. At X 9000 create a clear threshold arch with cool shadow beyond it. Keep its opening at least 400 cm wide and tall enough for the camera.
3. From X 9000–11000 use human-scale walls and partial columns. From X 11000–12600 use taller columns, heavier arches, and larger gaps. Near X 13200–14000 use the largest arch/portal and strong symmetry. This creates escalation without extending the route.
4. Keep the inscription near `(9700,-210)` lit and reachable. Keep the lever near `(10500,-250)` visible from the main route and leave its interaction radius clear.
5. Build visual gate art around `(11100,180,220)` but never replace or delete `RuinsGateCollision`. The assigned `Gate Visual Mesh` hides when the lever completes; surrounding frame pieces must remain. Test the open gap at full character and camera width.
6. Keep skeleton spawn zones near X 9400, 10100, and 11900 free of rubble, steep steps, and non-walkable gaps. Use rubble around the perimeter to imply collapse.
7. Layer moss on top-facing/outer surfaces, damp grime low on walls, and sparse vegetation in joints. Add rubble where a visibly broken wall could plausibly have fallen. Random decals are not a substitute for cause and effect.
8. Contrast warm torches at turns, inscription, lever, and gate with cool ambient fill. Avoid placing a torch every identical distance.
9. The portal at X 14000 is the Phase 5 boss entrance. Preserve the boss-owned trigger and invisible gate/blockers. Dress the chamber beyond it symmetrically, keep the central arena clear, and follow `PHASE_5_MITTENS_BOSS.md` for throne, cat, and encounter setup.

# F. Lighting

The runtime supplies a late-afternoon directional light, skylight, sky atmosphere, volumetric height fog, two village warm lights, and two ruins warm lights. Class Defaults expose safe global starting controls: Sun Intensity `3.2`, Sky Light Intensity `0.55`, and Fog Density `0.018`. Treat these as starting points, not calibrated final values.

1. In Play/Simulate, first check exposure. White plaster should retain detail and ruins should not crush into featureless black. Add one unbound Post Process Volume in the level and constrain auto-exposure (a narrow range such as EV100 min/max separated by 1–2 stops) only after testing interiors and exteriors. Exact EV depends on asset materials.
2. Tune sun intensity roughly **2–5 lux in the component's current unit presentation** and color from warm cream to pale gold. Look for readable faces, long soft-looking late-day shadows, and no orange color cast over every surface.
3. Tune skylight roughly **0.35–0.8**. Raise it if forest/ruins materials become black; lower it if all areas look flat. Shadows need information, not equal brightness.
4. Tune fog density roughly **0.008–0.025**. Fog should separate tree rows and catch ruin light, but the next objective must remain readable. If objects appear washed out at arm's length, reduce density before adding contrast.
5. Keep village practical lights warm (about 2200–3200 K if using temperature), local, and motivated by windows/lanterns. Keep forest mostly sun/sky/fog. Use warm torches against cool ruins ambience.
6. Use restrained bloom. A torch may glow; stone edges and the entire sky should not. Avoid strong vignette, chromatic aberration, or saturation that changes asset colors.
7. Judge lighting from gameplay camera at spawn, mid-forest, survivor, gate, and boss threshold—not only from editor free camera. Toggle **Game View** (`G`) and inspect moving characters in every zone.

Do not enable hardware ray tracing. Keep Lumen GI/reflections, software distance fields, Nanite project support, and Virtual Shadow Maps as configured.

# G. Performance

1. Enable Nanite on large opaque static meshes with dense geometry (rocks, ruins, substantial trunks) when the asset supports it. Do not blindly enable it on translucent effects or every masked grass card.
2. Use foliage instances for repeated grass, ferns, bushes, and non-hero trees rather than hundreds of separate actors. Hand-place hero silhouettes and clue props.
3. Start with appearance, then use **Stat FPS**, **Stat GPU**, and **ProfileGPU** in representative views. Optimize only a measured problem.
4. Give collision only to surfaces that affect play: route floor, gate blocker, major boundary trunks/walls, and deliberate blockers. Set small foliage, rubble, decals, and decorative C++ art to **No Collision**. Complex collision on every plant wastes time and can break navigation.
5. Use simple collision for large boundaries. Never use “complex as simple” on dense Nanite meshes without a tested need.
6. Keep material families shared and tune material instances. Avoid unique 4K materials for every prop. Favor 2K for ordinary props and reserve higher resolution for large, close surfaces.
7. Cull tiny foliage before it becomes subpixel; keep larger silhouette trees visible far enough to hide the world edge. Adjust foliage density in modest steps rather than halving quality everywhere.
8. After collision changes, show navigation with `P` and test every enemy area. Green coverage and stable combat matter more than one extra rubble pile.
9. Check Lumen Scene and Virtual Shadow Map visualization only when diagnosing leaks/noise. First fix gaps, overlapping surfaces, extreme emissive values, or excessive lights rather than disabling the rendering features.

# H. Final visual checklist — “Does this still look like a prototype?”

## Whole route

- [ ] No Engine cube/cylinder is visible from the gameplay camera; fallback geometry is understood as non-final.
- [ ] No floating foliage, rocks, walls, decals, props, NPCs, or enemies.
- [ ] Ground is continuously covered and transitions do not expose rectangular floor edges.
- [ ] Lighting has coherent direction, believable shadow detail, and restrained bloom/exposure.
- [ ] Each gameplay view contains foreground, midground, and background detail without cluttering the player.
- [ ] The intended route is obvious from path value, framing, light, and landmarks.
- [ ] No giant empty spaces or visible world edge/horizon gaps.
- [ ] Village, forest, and ruins share compatible scale, texel density, realism, and saturation.
- [ ] Hero, NPC, goblin/slime/skeleton models and animation quality fit the environment.
- [ ] No accidental collision snags; enemy navigation remains green and encounters have space.

## Story and progression

- [ ] Mildred reads immediately and her area feels warmer/cozier than the rest of the village.
- [ ] Village has multiple visible buildings and clustered domestic detail without becoming explorable scope.
- [ ] Forest becomes cooler, denser, and more ominous while the trail stays readable.
- [ ] Dead goblin, scratched tree, damaged cart, and survivor form a readable escalating tableau.
- [ ] Ruins use plausible modular construction, rubble, moss, dampness, and increasing architectural scale.
- [ ] Inscription and lever are visible/reachable; closed gate blocks and open gate is fully walkable.
- [ ] Warm ruin lights have visible sources and contrast with cool ambience.
- [ ] Boss entrance is the route's strongest silhouette; crossing it starts Mittens exactly once and does not unlock pickup.

## Required play checks

- [ ] Complete the route in order from Mildred to chamber threshold.
- [ ] Attempt to sequence-break every trigger.
- [ ] Fight at each spawn while watching camera collision, foliage occlusion, and navigation.
- [ ] Pull the lever and verify both visual and collision gate behavior.
- [ ] Restart after death and confirm no decorative/runtime duplication.
- [ ] Capture five gameplay-camera screenshots and compare them together for consistency.

## What Codex cannot finish

Codex can provide source-controlled hooks, renderer defaults, coordinates, folder conventions, and explicit procedures. It cannot select the best mesh from a visual library, accept licenses on the owner's behalf, import restricted binary assets, judge composition/material mismatch in the rendered scene, hand-place every prop around final mesh bounds, tune exposure/fog to final textures, validate animation aesthetics, or visually review PIE without Unreal Engine and the installed assets. Those tasks require a person looking at the actual rendered scene. This pass was source-reviewed only; it was not compiled or visually/PIE tested because Unreal Engine 5.8 is unavailable in the development environment.

## Risks to avoid

- Do not delete/move the route collision, triggers, nav bounds, encounter actors, lever, or gate collision while decorating.
- Do not place blocking vendor collision across the 350–450 cm route or combat clearings.
- Do not edit vendor master materials or reorganize dependencies outside Content Browser.
- Do not combine many free packs merely because they are free.
- Do not hide weak composition with heavy fog, bloom, darkness, or color grading.
- Do not start the boss room/boss logic during this pass; only dress its approach.
