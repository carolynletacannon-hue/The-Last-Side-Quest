# Phase 3 — Interaction, dialogue, and quest setup

Phase 3 is source-only: the C++ HUD creates its UMG dialogue/objective view at runtime, and the
player owns an `E` / gamepad Face Button Left interaction action. No Widget Blueprint, Input Action,
Data Table, or Level Blueprint logic is required. Blueprint children only supply presentation and the
editable content below.

## Editor setup (click by click)

1. Build the `TheLastSideQuestEditor` target, open the project, and let Unreal compile any Blueprint
   references.
2. In the Content Browser, make `Content/Phase3Test`. Right-click **Blueprint Class**, expand **All
   Classes**, choose **QuestInteractableActor**, and create each actor in the table below.
3. Open each Blueprint, click **Class Defaults**, and fill **Interaction**, **Required Step**, **Result
   Step**, **Advance Quest**, and the ordered **Dialogue** array. Add a Static Mesh or Skeletal Mesh
   component only to make the placeholder visible. Its collision must overlap Pawn queries (the default
   `OverlapAllDynamic` preset is sufficient).
4. For each route row in the trigger table, right-click **Blueprint Class**, choose
   **QuestProgressTrigger**, set its two quest steps in **Class Defaults**, then place and scale it across
   the test route.
5. Drag the actors and triggers into any test map in table order. Keep interactables within 225 Unreal
   units of the walking path. Press Play; the project GameMode already supplies the correct player,
   GameState, and HUD.

## Interactable values

| Blueprint | Label | Required → Result | Advance | Dialogue lines (`Speaker: Text`) |
|---|---|---|---|---|
| `BP_Mildred_Start` | Talk to Mildred | Talk To Mildred → Talk To Guard | Yes | Mildred: “My Mittens has gone missing. Would you find him for me, dear?” / Player: “I just killed the Dark Lord.” / Mildred: “That's lovely. Mittens likes chicken.” / Quest: “The Last Side Quest — Reward: 3 Gold” |
| `BP_Guard` | Talk to Guard | Talk To Guard → Follow Forest Clue | Yes | Guard: “Hero! The kingdom owes you everything.” / Player: “Have you seen a cat?” / Guard: “…Orange?” / Player: “Yes.” / Guard: “Went into the forest.” |
| `BP_Goblin` | Talk to goblin | Talk To Goblin → Enter Ruins | Yes | Goblin: “THE BEAST.” / Player: “…The cat?” / Goblin: “DO NOT SPEAK ITS NAME.” |
| `BP_Inscription` | Read inscription | Open Ruins Gate → Open Ruins Gate | **No** | Inscription: “HE WHO ENTERS SHALL FACE THE DEVOURER.” / Player: “Mittens?” |
| `BP_RuinsLever` | Pull lever | Open Ruins Gate → Reach Mittens | Yes | (leave empty) |
| `BP_MittensPickup` | Pick up Mittens | Pick Up Mittens → Return To Mildred | Yes | (leave empty) |
| `BP_Mildred_Return` | Return Mittens | Return To Mildred → Complete | Yes | Mildred: “Mittens! There you are!” / Player: “That creature has killed at least twelve people.” / Mildred: “Oh, he gets grumpy when he's hungry.” / Quest: “QUEST COMPLETE — THE LAST SIDE QUEST — Reward: 3 Gold” / Mildred: “Would you mind finding my other cat?” / Player: “No.” |

The inscription is deliberately optional and repeatable while the lever is available. Implement
`On Interaction Completed` on the lever or pickup Blueprint only for simple presentation such as
opening the placeholder gate or hiding the pickup mesh; do not advance quest state in Blueprint.

## Route trigger values

| Trigger | Required → Result |
|---|---|
| Forest clue | Follow Forest Clue → Talk To Goblin |
| Ruins entrance | Enter Ruins → Open Ruins Gate |
| Mittens chamber threshold | Reach Mittens → Pick Up Mittens |

The chamber threshold represents reaching the future boss for this phase. It does not spawn, start,
or implement the boss. In Phase 4, gate the pickup's collision/visibility until Mittens is defeated;
Phase 3 intentionally leaves it available so the complete quest flow can be tested.

## Complete test flow

Walk to each actor and verify the gold `[E]` prompt only appears at the actor valid for the current
objective. Press E once to open dialogue and once per line to continue. While dialogue is visible,
verify movement, jumping, and melee are blocked but camera orbit still works; after the final line,
verify those controls and normal interaction prompts return immediately. Follow the three trigger
volumes at their positions, read the inscription, pull the lever, cross the chamber threshold, pick up
the placeholder Mittens, and return to the second Mildred actor. Confirm the objective ends at
`Quest complete — Reward: 3 Gold`. Also verify movement/camera, melee, health/death, and R restart
still behave as before, including death during dialogue and R restart after death.
