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
