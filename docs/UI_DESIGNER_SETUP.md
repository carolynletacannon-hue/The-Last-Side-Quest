# UI Designer Setup

The HUD gameplay logic is already connected. The Unreal work is presentation only: create two Widget Blueprints, design them, and select them on a HUD Blueprint.

## 1. Create the visual widgets

Create `WBP_InteractionDialogue` with parent class `InteractionDialogueWidget` and `WBP_CombatStatus` with parent class `CombatStatusWidget`.

The C++ base classes automatically update specially named widgets. In the Designer, enable **Is Variable** and use these exact names. Every hook is optional, so the project still runs while the design is unfinished.

### WBP_InteractionDialogue hooks

| Exact widget name | Widget type | Supplied content/state |
| --- | --- | --- |
| `ObjectiveText` | Text Block | Current quest objective |
| `PromptText` | Text Block | Interaction prompt including `[E]` |
| `DialoguePanel` | Border | Shown only during dialogue |
| `SpeakerText` | Text Block | Current speaker |
| `DialogueText` | Text Block | Current dialogue line |
| `SubtitlePanel` | Border | Shown for non-modal gameplay subtitles |
| `SubtitleSpeakerText` | Text Block | Subtitle speaker |
| `SubtitleBodyText` | Text Block | Subtitle line |
| `QuestCompletePanel` | Border | Quest-complete presentation |
| `FadePanel` | Border | Ending fade; its color alpha is controlled by code |
| `CreditsPanel` | Vertical Box | Credits presentation |

Blueprint events `OnDialogueVisibilityChanged` and `OnPromptVisibilityChanged` are available for optional fade, slide, glow, or scale animations.

### WBP_CombatStatus hooks

| Exact widget name | Widget type | Supplied content/state |
| --- | --- | --- |
| `PlayerHealthBar` | Progress Bar | Player health fraction |
| `PlayerHealthText` | Text Block | Player health numbers |
| `BossPanel` | Border | Shown only during the boss encounter |
| `BossHealthBar` | Progress Bar | Boss health fraction |
| `BossNameText` | Text Block | Boss title |
| `BossHealthText` | Text Block | Boss health numbers |
| `DamageFlash` | Border | Full-screen hit flash; opacity controlled by code |
| `DefeatedPanel` | Border | Shown when the player dies |

Blueprint events `OnPlayerHealthChanged`, `OnBossHealthChanged`, `OnDamageFlashChanged`, and `OnDefeatedChanged` expose the same values for animations and material effects.

## 2. Tell the game to use them

1. Create Blueprint Class `BP_SideQuestHUD` with parent `SideQuestHUD`.
2. In Class Defaults, set **Interaction Widget Class** to `WBP_InteractionDialogue`.
3. Set **Combat Widget Class** to `WBP_CombatStatus`.
4. Create `BP_SideQuestGameMode` from `SideQuestGameMode` if one does not already exist.
5. In its Class Defaults, set **HUD Class** to `BP_SideQuestHUD`.
6. Set the level's GameMode Override to `BP_SideQuestGameMode` in World Settings.

## Suggested art direction

- Charcoal carved-stone panels, antique-gold trim, and warm ivory body text.
- Deep crimson player health and violet-blue magical boss health.
- Keep textures on Borders/Images behind the named value widgets; code does not replace styling.
- Use a readable serif for dialogue and a more decorative font only for names and titles.
- Set dialogue and boss panels to **Not Hit-Testable (Self & All Children)** so they never block gameplay input.
- Anchor the player bar bottom-left, dialogue bottom-center, boss bar top-center, objective top-left, and prompt just below screen center.

If no Blueprint classes are selected, the code-generated fallback HUD remains active, so styling can be done one piece at a time without breaking the game.
