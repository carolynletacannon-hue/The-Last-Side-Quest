#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Quest/SideQuestTypes.h"
#include "TimerManager.h"
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
    UFUNCTION(BlueprintPure, Category = "Ending") EEndingPresentationState GetEndingState() const { return EndingState; }
    UFUNCTION(BlueprintPure, Category = "Ending") bool HasEndingBegun() const { return EndingState != EEndingPresentationState::None; }
    UFUNCTION(BlueprintPure, Category = "Ending") float GetFadeOpacity() const;
    bool BeginEndingSequence(class ASideQuestCharacter* Player, class AQuestInteractableActor* Mildred);
    void FinishFinalDialogue();

    UPROPERTY(BlueprintAssignable, Category = "Quest") FQuestStepChanged OnQuestStepChanged;
    /** Cosmetic only: a Blueprint GameState may play quest-accepted/objective-update presentation. */
    UFUNCTION(BlueprintImplementableEvent, Category = "Quest|Presentation")
    void PlayQuestStepPresentation(ESideQuestStep NewStep);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Quest")
    TMap<ESideQuestStep, FText> ObjectiveText;

private:
    void BeginFinalDialogue();
    void ShowCredits();
    void SetEndingState(EEndingPresentationState NewState);

    UPROPERTY(VisibleInstanceOnly, Category = "Quest")
    ESideQuestStep QuestStep = ESideQuestStep::TalkToMildred;
    UPROPERTY(VisibleInstanceOnly, Category = "Ending") EEndingPresentationState EndingState = EEndingPresentationState::None;
    UPROPERTY() TObjectPtr<class ASideQuestCharacter> EndingPlayer;
    UPROPERTY() TObjectPtr<class AQuestInteractableActor> EndingMildred;
    FTimerHandle EndingTimer;
    float FadeStartedAt = 0.0f;
};
