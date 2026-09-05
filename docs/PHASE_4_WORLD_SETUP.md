# Phase 4 — Village, forest, and ruins

## What is source controlled

`APhase4World` builds the entire compact outbound route when play begins. `ASideQuestGameMode`
spawns it automatically unless a Phase 4 world actor already exists. The result is playable in an
otherwise empty map and contains:

- a compact village start, Mildred at the opening composition, and the guard beside the obvious exit;
- a gently bending forest path, five normal enemies, forest-clue trigger, and terrified survivor;
- an imposing ruins approach, entrance trigger, inscription, lever, blocking gate, three skeletons,
  and the future boss-chamber threshold;
- collision under the full path, runtime-generated navigation bounds, warm village/torch pools,
  height fog, and deliberately simple landmark/route silhouettes;
- strict quest-step checks on all interactions and triggers, so walking ahead cannot skip the route;
- an authoritative lever completion delegate that removes both gate collision and its visual, avoiding
  Level Blueprint quest logic and preventing the closed gate from becoming a soft lock.

The source fallback uses only built-in Engine meshes. This is intentional so gameplay can be reviewed
without redistributing third-party content, but it is placeholder art—not a claim that production art
was imported.

## Route and authored placement

The walk runs about 14,000 Unreal units and is intended to take roughly 2–3 minutes without combat.
Its required order is:

1. **Village:** spawn → Mildred → guard → warm-lit forest exit.
2. **Forest:** clue threshold → five encounters (three goblin slots, two slime slots) → survivor near
   the dead-body/scratched-tree dressing zone → ruins approach.
3. **Ruins:** entrance threshold → optional/repeatable inscription → lever → gate → three skeleton
   slots → chamber threshold. The threshold advances to `Pick Up Mittens` for Phase 3 compatibility,
   but Phase 4 places no cat, boss, pickup, ending, or credits.

## Required art pass in Unreal Editor

This is the unavoidable editor work because licensed binaries are not present in the repository.

1. Build and open the project. In **Content Browser**, create `Content/World/Phase4`.
2. Right-click → **Blueprint Class** → **All Classes** → `Phase4World`; name it `BP_Phase4World`.
3. Open it → **Class Defaults**. Under **Phase 4 | Art**, assign a modular ground mesh, vertical
   dressing mesh, and the three cohesive PBR material instances described in `ASSET_PLAN.md`.
4. Under **Phase 4 | Cast**, assign the five Phase 3 Blueprint children. Under **Enemies**, assign the
   goblin, slime, and skeleton children. Compile and save.
5. Open the project's gameplay map. If a C++ `Phase4World` was already placed, delete it. Drag
   `BP_Phase4World` to world origin `(0, 0, 0)`. The GameMode will detect it and will not spawn a
   duplicate.
6. Replace the broad fallback dressing with licensed village buildings/props at X `0–3200`, foliage,
   roots, rocks, scratches and a dead-goblin tableau at X `3200–8800`, and broken arches, masonry,
   moss, decals, torches and rubble at X `8800–14000`. Keep the center path and gameplay actors clear.
7. Add a **Sky Atmosphere**, **Directional Light**, and restrained unbound **Post Process Volume**.
   Keep Lumen, Virtual Shadow Maps, and software ray tracing settings unchanged. Do not enable hardware
   ray tracing.
8. Select the runtime-generated nav bounds during PIE and press **P**. If any enemy encounter is not
   green, stop PIE, place a **Nav Mesh Bounds Volume** spanning `(0–14000, ±900)`, then build navigation.
9. Play from Mildred through the chamber threshold. Confirm the gate disappears only after the lever
   completes and that its former opening is walkable.

## Phase 4 test checklist

1. Move, orbit the camera, jump, and attack before speaking to Mildred.
2. Complete Mildred and guard dialogue; confirm dialogue locks movement/jump/attack, not camera.
3. Cross the clue trigger and defeat or evade all five forest enemies; confirm chase stays on navmesh.
4. Confirm the damaged clearing reads visually, then talk to the terrified goblin.
5. Cross the ruins entrance, read the inscription, and defeat the three skeletons.
6. Try the closed gate, pull the lever, and confirm collision and visual obstruction are both removed.
7. Cross the chamber threshold and confirm the objective changes to `Pick up Mittens`, with no boss
   encounter yet.
8. On a separate run, die during combat, press **R**, and confirm the level and quest restart cleanly.
9. Attempt to run ahead before each quest beat; no out-of-order trigger or interaction should advance.
