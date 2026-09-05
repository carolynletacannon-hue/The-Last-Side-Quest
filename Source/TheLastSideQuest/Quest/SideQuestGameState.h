#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Quest/SideQuestTypes.h"
#include "SideQuestGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestStepChanged, ESideQuestStep, NewStep);

/** The deliberately small, single linear quest for this game. */
UCLASS(Blueprintable)
class THELASTSIDEQUEST_API ASideQuestGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ASideQuestGameState();

    UFUNCTION(BlueprintPure, Category = "Quest") ESideQuestStep GetQuestStep() const { return QuestStep; }
    UFUNCTION(BlueprintPure, Category = "Quest") FText GetObjectiveText() const;
    UFUNCTION(BlueprintCallable, Category = "Quest") bool TryAdvanceQuest(ESideQuestStep ExpectedStep, ESideQuestStep NewStep);

    UPROPERTY(BlueprintAssignable, Category = "Quest") FQuestStepChanged OnQuestStepChanged;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
    TMap<ESideQuestStep, FText> ObjectiveText;

private:
    UPROPERTY(VisibleInstanceOnly, Category = "Quest")
    ESideQuestStep QuestStep = ESideQuestStep::TalkToMildred;
};
