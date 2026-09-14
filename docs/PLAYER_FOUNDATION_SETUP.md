# Player Foundation Setup

Issue #1 provides a source-only playable third-person character. Its built-in Enhanced Input mappings are:

- **WASD / left stick:** move relative to camera yaw
- **Mouse / right stick:** orbit the camera
- **Space / gamepad bottom face button:** jump

The project game mode automatically uses `ASideQuestCharacter`, so no Blueprint is required to test it in a map containing a `PlayerStart` and walkable collision.

## Optional editor presentation setup

1. Create a Blueprint subclass of `ASideQuestCharacter`.
2. Assign the licensed player skeletal mesh and animation Blueprint on its inherited Mesh component.
3. Tune Character Movement and the inherited Camera Boom for the final asset's proportions.
4. If input assets are preferred for editor iteration, assign a replacement mapping context and matching input actions in the Blueprint defaults.
5. Create a Blueprint subclass of `ASideQuestGameMode`, set its Default Pawn Class to that character Blueprint, and select it in Project Settings or World Settings.

These steps are asset/presentation hooks only. Movement, camera, jump, and default mappings remain in C++.

## Sword and combat animation setup

The character owns a `SwordMesh` component and automatically moves it between two sockets while drawing and sheathing.

1. On the protagonist skeleton, create a back socket named `Sword_Back` and a hand socket named `Sword_Hand`. The names can instead be changed in the character Blueprint defaults.
2. Assign the imported sword static mesh to the inherited `SwordMesh` component and adjust the socket transforms until the sword sits correctly in both locations.
3. Create and assign montages for `Draw Reach`, `Draw Ready`, `Sheathe Sword`, and `Sheathe Hand Down`.
4. Set each corresponding duration property to the exact length of its clip. The sword transfers to the hand between the two draw clips and transfers to the back between the two sheathe clips.
5. Assign the combat locomotion sequences in the Animation Blueprint and use `Is Sword Drawn` to switch between ordinary and sword-in-hand locomotion. `Is Weapon Transitioning` is available when transition-specific Blueprint behavior is needed.

Pressing light or heavy attack while the sword is sheathed automatically plays both draw clips, transfers the sword, and then performs the queued attack. After four seconds without an attack or dodge, both sheathing clips play and the sword returns to the back socket. The delay is editable with `Auto Sheathe Delay`.
