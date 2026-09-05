# Content layout

These empty source-control folders reserve the project convention for licensed assets added through Unreal Editor:

- `Environment/Village`, `Environment/Forest`, `Environment/Ruins`
- `Characters`, `Enemies`, `Props`, `Materials`, `VFX`, `Audio`

Unreal's Content Browser may hide an empty folder until its first `.uasset` is added. Never commit purchased or restricted source files merely to populate these folders. Keep each vendor pack in its original imported subfolder until references are stable; migrate selected production assets into the area folders only through Unreal's **Migrate** tool, and use **Fix Up Redirectors** after intentional moves.
