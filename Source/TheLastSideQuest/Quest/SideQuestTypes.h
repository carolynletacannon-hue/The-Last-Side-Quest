#pragma once

#include "CoreMinimal.h"
#include "SideQuestTypes.generated.h"

UENUM(BlueprintType)
enum class ESideQuestStep : uint8
{
    TalkToMildred,
    TalkToGuard,
    FollowForestClue,
    TalkToGoblin,
    EnterRuins,
    OpenRuinsGate,
    ReachMittens,
    FightMittens,
    PickUpMittens,
    ReturnToMildred,
    Complete
};

UENUM(BlueprintType)
enum class EEndingPresentationState : uint8
{
    None,
    QuestComplete,
    FinalDialogue,
    Fading,
    Credits
};

USTRUCT(BlueprintType)
struct FSideQuestDialogueLine
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Speaker;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = "true"))
    FText Text;
};
