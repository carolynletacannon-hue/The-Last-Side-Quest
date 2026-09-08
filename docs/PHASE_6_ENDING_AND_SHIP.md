# Phase 6 — Ending, credits, and ship preparation

## A. What Phase 6 implemented

Phase 6 finishes the one quest without adding another gameplay system. The same source-spawned
Mildred actor handles both the opening and the strictly gated return interaction. Her first three
return lines complete the authoritative `ReturnToMildred → Complete` transition. A timed,
source-created UMG card then displays **QUEST COMPLETE**, **THE LAST SIDE QUEST**, and
**Reward: 3 Gold** before the existing dialogue presentation delivers the two-line final joke.

The Game State owns the small ending sequence (`QuestComplete`, `FinalDialogue`, `Fading`, and
`Credits`). This gives the HUD and player one authoritative state to consult. During it, movement,
jumping, attacks, ordinary interaction, damage, objectives, boss UI, health UI, and death UI cannot
compete with the ending. After “No.”, the HUD fades to black and shows static credits. `R` or the
controller Menu button reloads the whole level; Escape or controller B quits from credits. A packaged
build exits normally. In PIE, Unreal may stop the play session or may ignore the platform quit request,
depending on editor play settings; use the editor's **Stop** button if necessary.

`AQuestInteractableActor` exposes cosmetic Blueprint events for returning Mittens, quest completion,
the final joke, and the fade transition. They may play animation, audio, particles, or camera work, but
none controls quest progress. Existing combat, gate, boss, meow, defeat, and pickup hooks remain the
places to assign their corresponding presentation.

## B. Complete game flow

There is one ordered source path:

1. `TalkToMildred` — hear the opening request.
2. `TalkToGuard` — ask where the white cat went.
3. `FollowForestClue` — cross the forest clue threshold.
4. `TalkToGoblin` — hear the survivor's warning.
5. `EnterRuins` — cross the ruins threshold.
6. `OpenRuinsGate` — optionally read the inscription, then pull the required lever.
7. `ReachMittens` — approach the chamber.
8. `FightMittens` — the boss-owned entrance starts the fight.
9. `PickUpMittens` — only Mittens' defeat unlocks this interaction.
10. `ReturnToMildred` — only the defeated cat's pickup reaches this step.
11. `Complete` — finish Mildred's first three return lines, see the reward, hear the other-cat joke,
    fade to black, and reach credits.

Every source transition must move to the immediately following enum value. Actors and thresholds also
check their required step. Consequently the guard, clue, goblin, entrance, lever, boss, pickup, and
return cannot advance early. The chamber starts rather than bypasses combat; a living Mittens rejects
interaction; and credits are entered only by completing the final two-line dialogue.

## C. Ending test (Play In Editor)

1. Compile the Editor target, open the gameplay map, and choose **Play**.
2. Complete the route through the boss. Reduce Mittens to zero health and advance the “Meow.” line.
3. Confirm the objective is **Pick up Mittens**, approach the resting cat, and press `E`. Confirm the
   cat disappears, its pickup presentation fires once, and the objective becomes **Return Mittens to
   Mildred**.
4. Return along the route to the original Mildred. Confirm `[E] Talk to Mildred` appears only now, and
   press it.
5. Advance exactly these lines one press at a time: “Mittens! There you are!”, “That creature has
   killed at least twelve people.”, and “Oh, he gets grumpy when he's hungry.”
6. Confirm the ordinary dialogue clears and the centered reward card automatically appears with all
   three required text elements. Confirm movement, jump, attack, damage, and unrelated prompts remain
   disabled. Do not press anything; the card should clear itself after about 2.75 seconds.
7. Confirm the final dialogue then starts with “Would you mind finding my other cat?” Press once and
   confirm the immediate next line is the player's short “No.” Press once more.
8. Confirm a roughly 1.25-second fade reaches full black, with no objective, health, death, or boss
   UI visible, then shows the static title, creator, engine credit, optional attribution reminder,
   “Thanks for playing.”, and controls.
9. At credits press `R` (or controller Menu). Confirm a full level reload: full player health, opening
   objective, closed ruins gate, all enemies alive, fresh undefeated Mittens, and no ending UI.
10. Reach credits again and press Escape (or controller B). In a packaged build the process should
    exit. In PIE behavior can vary; if PIE does not close, click **Stop** and do not treat that alone as
    a packaged-build failure.

Also attempt to interact with Mildred immediately after the opening and before pickup. No return
dialogue or completion card should be available.

## D. Full-game test checklist

- [ ] Start a new level and see `Talk to Mildred`, full health, no boss UI, and no credits.
- [ ] Test WASD/controller movement, jump, mouse/controller camera, and camera collision.
- [ ] Talk to Mildred; confirm one input advances one line and movement/jump/sword are blocked.
- [ ] Confirm quest acceptance and 3 Gold promise presentation, then talk to the guard.
- [ ] Draw/use the sword against forest enemies; verify hits, damage, enemy death, and presentation hooks.
- [ ] Take damage; verify health and hit feedback. In a separate run, die and verify `R`/Menu reload.
- [ ] Follow the readable forest route; verify the clue advances only at `FollowForestClue`.
- [ ] Talk to the goblin only at `TalkToGoblin`; verify the three lines and next objective.
- [ ] Enter the ruins only at `EnterRuins`; optionally read the inscription without advancing.
- [ ] Pull the lever only at `OpenRuinsGate`; verify gate visual and collision both clear.
- [ ] Fight the ruins enemies and check navigation, collision, attack readability, and no soft lock.
- [ ] Cross the chamber threshold at `ReachMittens`; verify it starts `FightMittens` once and closes
      the boss entrance without unlocking pickup.
- [ ] Verify the title, boss health, pounce, scratch combo, chaos dash, hit feedback, and player death.
- [ ] Defeat Mittens; confirm attacks and boss UI stop, the gate opens, “Meow.” appears, and pickup is
      enabled only now.
- [ ] Pick up Mittens once; verify the world cat hides and `ReturnToMildred` appears.
- [ ] Return to the same Mildred, play the required three lines, and see the distinct 3 Gold card.
- [ ] Verify the automatic transition to the other-cat question and the immediate, final “No.”
- [ ] Verify fade, clean credits, attribution placeholder, restart, and packaged-build quit.
- [ ] Complete one uninterrupted 10–15 minute packaged-build playthrough with no sequence break,
      collision blocker, navigation failure, stale prompt, duplicate trigger, or overlapping UI.

## E. Packaging guide — Windows

The exact toolbar wording or location can move between Unreal Engine point releases. These steps use
the usual UE5 names; if UE 5.8 labels differ, search **Project Settings** for the bold setting name and
use the platform/package command shown by the editor rather than guessing a replacement.

1. On a Windows machine install Unreal Engine **5.8**, Visual Studio with **Game development with
   C++**, the Windows SDK recommended by that engine install, and any prerequisites reported by the
   Unreal installer.
2. Right-click `TheLastSideQuest.uproject` and generate Visual Studio project files if needed. Build
   the `TheLastSideQuestEditor` target for Win64 Development, then open the `.uproject` in UE 5.8.
3. Create/save the final gameplay level as `/Game/Maps/L_LastSideQuest` if the project does not yet
   contain a production map. Place the configured `BP_Phase4World` at world origin, or leave the map
   empty to use the C++ fallback world. Add a Player Start only if its final placement has been tested.
4. Open **Edit → Project Settings → Maps & Modes**. Set **Game Default Map** and **Editor Startup Map**
   to `L_LastSideQuest`. Confirm the default GameMode is `SideQuestGameMode` (the config already sets
   its global fallback). Save all.
5. In **Project Settings → Packaging**, add `L_LastSideQuest` to **List of maps to include in a
   packaged build** if that list is present. Leave **Use Pak File / Io Store** at UE's tested defaults.
   Ensure no missing redirectors or editor-only references are reported. Do not enable ray tracing.
6. Choose **Platforms → Windows** (wording may appear as **Package Project → Windows**), select
   **Shipping** for the release candidate, and choose **Package Project**. Use **Development** first
   only when logs/debugging are needed. Pick an empty output folder such as
   `Builds/Windows-Shipping`; do not package into `Content` or commit generated output.
7. Wait for completion and inspect **Output Log** / **Message Log** for errors. The output is normally
   under the folder selected in the dialog, often with a `Windows` or `WindowsNoEditor` child.
8. Launch the generated `TheLastSideQuest.exe`, not the executable under the engine directory. Test
   on a machine without the editor if possible.
9. When sharing, zip the entire generated Windows output folder. Keep the `.exe`, `Content`,
   `Engine`, configuration, `.pak`/`.ucas`/`.utoc`, prerequisites, and all sibling DLLs together with
   their original relative paths. Never send only the `.exe`.
10. Unzip to a new location and perform the full checklist above. A successful editor session is not
    proof that cooking or the packaged executable works.

The repository deliberately does not name a nonexistent map in `DefaultEngine.ini`; setting a stale
asset path would be a packaging/startup blocker. Save the real final map first, then make the settings
in steps 3–5 source controlled.

## F. Mac development notes

Install the UE 5.8 macOS editor and the Xcode version required by Epic for that engine build, accept
the Xcode license, and install command-line tools. Generate/open the Xcode workspace from the
`.uproject`, build the `TheLastSideQuestEditor` Mac Development target, and let Unreal rebuild modules.
Use the same final map and run PIE/full-game checks on Mac.

Keep Windows as the shipping target: do not replace the DX12 Windows setting, enable hardware ray
tracing, or commit Mac-only absolute paths. The renderer config intentionally retains Lumen, Virtual
Shadow Maps, Nanite support, mesh distance fields, and software rendering paths. Validate final visual
quality separately on both platforms because feature support, shader compilation, and performance can
differ. Build the Windows package on Windows with the Windows toolchain; do not assume a Mac editor
test proves the Windows cook or binary.

## G. Final release checklist

- [ ] No visible placeholder cubes, cylinders, spheres, stretched cat proxy, or source fallback art.
- [ ] Real, licensed player, Mildred, guard, goblin, enemies, and pristine, extremely fluffy white cat with a pink jeweled collar assigned.
- [ ] Locomotion, sword, hit, death, boss attack, defeat/rest, pickup, and NPC animations assigned.
- [ ] Cohesive PBR materials and appropriate Nanite settings assigned; hardware ray tracing remains off.
- [ ] Village, forest, ruins, and return route fully dressed without hiding gameplay.
- [ ] Audio/VFX assigned to existing acceptance, objective, enemy, gate, boss, meow, pickup, return,
      completion, and ending hooks.
- [ ] Boss room composition, lighting, fog, throne, VFX, collision, and readability polished.
- [ ] No quest sequence breaks or duplicate/legacy boss progression triggers.
- [ ] No collision blockers or camera snags on the complete outbound and return route.
- [ ] No obvious navigation failures in every combat space.
- [ ] Credits spelling and **Created by Carolyn Cannon** confirmed.
- [ ] `docs/ASSET_PLAN.md` and credits attribution updated for every license that requires it.
- [ ] Shipping Windows package completes and launches on a clean machine.
- [ ] One full packaged playthrough from opening line through credits and restart completed.

## H. What Codex could not verify

This source pass cannot substitute for the installed Unreal toolchain and final licensed assets. The
following still require human/editor work:

- **Unreal compilation:** build the UE 5.8 Editor, Mac, and Windows targets and resolve any engine-
  version API/compiler diagnostics.
- **Play In Editor:** execute every ending and full-game step, including timing, input debounce,
  sequence-break attempts, death, and restart state reset.
- **Visual inspection:** replace source primitives, inspect HUD readability at common desktop
  resolutions, verify fade/credit layering, lighting, materials, collision, and camera composition.
- **Animation inspection:** assign and review all player, NPC, enemy, and Mittens clips/transitions.
- **Audio inspection:** assign licensed sounds/music to cosmetic hooks, check levels and transitions,
  and confirm required attribution.
- **Performance testing:** profile representative village, forest, ruins, and boss views on target
  Windows hardware and a development Mac.
- **Packaging:** create/save/select the production map, cook/package Windows Shipping, launch it on a
  clean computer, verify quit behavior, and complete a packaged playthrough.

Unreal Engine 5.8 is not available in the source environment used for this phase, so compilation, PIE,
visual/animation/audio review, gameplay balance, performance, and packaging are explicitly unverified.
Subject to those editor and release checks, the required gameplay is feature-complete in source.
