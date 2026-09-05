# The Last Side Quest — Game Design

## Premise

The player is a legendary adventurer who has already defeated the Dark Lord and saved the kingdom. One unfinished side quest remains: Mildred's missing orange cat, Mittens.

The joke is escalation. A mundane missing-cat errand becomes increasingly ominous until the player reaches a cathedral-scale boss room and discovers Mittens on the throne beneath the title:

**MITTENS — DEVOURER OF WORLDS**

## Target experience

- 10–15 minutes
- third-person 3D fantasy action RPG
- one linear quest
- visually polished small-world presentation
- comedic writing played completely straight by the world

## Route

### 1. Village
Warm, safe, recently victorious medieval village. Mildred asks the legendary hero to find Mittens.

Core beat:
- Mildred gives quest
- reward displayed as 3 Gold
- guard points toward forest

### 2. Forest
Readable storybook forest path with 4–6 lightweight enemies and increasingly absurd environmental evidence that Mittens is dangerous.

Core beats:
- first combat
- scratches/damage/clues
- terrified goblin survivor calls Mittens “the beast”
- trail leads to ruins

### 3. Ruins
Small ancient dungeon that dramatically overstates the stakes.

Core beats:
- 2–3 enemies
- one simple lever/gate progression interaction
- monumental inscription / boss foreshadowing
- entry into boss chamber

### 4. Mittens boss
A normal-looking orange domestic cat sits on a throne in a wildly cinematic chamber.

Boss title:
**MITTENS — DEVOURER OF WORLDS**

Required attacks:
- Pounce
- Scratch combo
- Chaos dash

Optional if time permits:
- Meow shockwave/screen shake

After defeat, Mittens stops fighting and becomes interactable. Player picks him up.

### 5. Return to Mildred
Return/cut back to the village.

Mildred is delighted. Player notes that the cat is horrifying. Mildred explains that he gets grumpy when hungry.

Quest complete:
**Reward: 3 Gold**

Mildred asks whether the player can find her other cat.

Player: **“No.”**

Cut to black. Credits.

## Core systems

Only build what this game needs:
- third-person locomotion/camera
- one melee sword attack
- player health/death/restart
- simple enemy AI and health
- interaction prompt
- linear dialogue
- current objective HUD
- quest-state progression
- boss health/AI
- ending/credits

## Explicitly out of scope

- inventory
- equipment
- crafting
- loot systems
- XP or leveling
- skill trees
- shops
- branching dialogue
- open world
- procedural quests
- multiplayer
- complex save system

## Presentation

Use Unreal Engine 5.8's modern desktop rendering stack and the art direction in `ART_DIRECTION.md`. The environment should be compact enough that detail density, lighting, materials, foliage, VFX and composition can carry the visual quality rather than relying on enormous map size.
