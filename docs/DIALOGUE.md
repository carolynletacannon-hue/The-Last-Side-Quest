# Dialogue Script

The playable dialogue is implemented in `Phase4World.cpp` and `MittensBoss.cpp`. This document records the intended delivery and presentation beats that accompany it.

## Comedy direction

- The protagonist is dry, exhausted, competent, and increasingly offended by the quest.
- Mildred is completely sincere and treats everything Mittens does as ordinary cat behavior.
- The guard knows more than he admits and has decided it is someone else's problem.
- The goblin treats Mittens as an ancient cosmic predator.
- Mittens behaves like a normal house cat despite possessing unreasonable power.
- No character deliberately delivers a punchline. Everyone plays the danger seriously.

## Implemented conversations

### Mildred — quest start

Mildred greets the hero immediately after her victory over the Dark Lord, establishes that Mittens is white, fluffy, and wearing a pink collar, and offers three gold for his return. The hero accepts with increasing disbelief. Mildred directs her to the north-gate guard and warns her to support Mittens' back legs when picking him up.

Quest card: **THE LAST SIDE QUEST — Find Mildred's cat — Reward: 3 Gold**

### Village guard

The guard insists on following protocol even though the hero saved his life yesterday. He recognizes Mittens from the description, mentions the screaming that followed the cat into the forest, and refuses to accompany the hero.

### Goblin survivor

The survivor calls Mittens “the Beast,” is horrified to learn that humans gave it a name, and becomes even more alarmed upon learning that it has a master. The goblin directs the hero to the ruins and explains what happened when Gribbo tried to pet it.

### Ruins inscription

Three warnings identify the White Devourer, its jeweled collar, and its appetite. The hero unsuccessfully tries to convince herself that the warnings are unrelated to Mildred's feeding schedule.

### Boss defeat and pickup

After defeat, Mittens immediately returns to normal cat behavior. The hero objects to being purred at, admits he is lucky to be fluffy, then remembers Mildred's warning about supporting his back legs when she picks him up.

### Return to Mildred

The hero reports the temple, goblins, prophecies, attempted murder, and lightning. Mildred dismisses each fact as normal cat behavior before the quest-completion presentation awards three gold.

### Final joke

Mildred begins asking the hero to find Mr. Whiskers, her other cat, who was last seen flying toward the mountains. The hero repeatedly refuses, and the game cuts to black.

## Presentation-only lines and barks

These should be delivered through Blueprint presentation events or a future non-blocking subtitle/bark function. They should not use the modal interaction dialogue during active combat.

- Boss reveal: **MITTENS — DEVOURER OF WORLDS** / Player: “Oh, come on.”
- Pounce: Player: “Bad cat!”
- Heavy player damage: Player: “Mildred owes me substantially more than three gold!”
- Chaos dash: Player: “WHY ARE YOU SO FAST?!” / Mittens: “Mrrrp.”
- Low boss health: Player: “That's right. Fear me.”
- Forest clue: Player: “Those are cat scratches. Those are... very large cat scratches.”
- Ruins entrance: Player: “Of course the missing cat went into the ancient cursed ruins.”
- Credits stinger: Mildred: “Mr. Whiskers?” / massive roar / Mildred: “There you are!”

## Optional enemy barks

- Goblin: “Not again!”
- Goblin: “It's wearing armor now!”
- Player: “I am not a cat.”
- Goblin: “Oh. Still bad!”
- Goblin: “Protect the food!”
- Player: “I'm not here for your food.”
- Goblin: “That's what the beast wanted!”

Enemy barks should be short, infrequent, and non-blocking so they do not interrupt combat.
