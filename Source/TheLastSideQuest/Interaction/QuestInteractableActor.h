#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SideQuestInteractable.h"
#include "Quest/SideQuestTypes.h"
#include "QuestInteractableActor.generated.h"

class USceneComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionFinished);

/** Configurable NPC, inscription, lever, or pickup. Presentation belongs in a thin Blueprint child. */
UCLASS(Blueprintable)
class THELASTSIDEQUEST_API AQuestInteractableActor : public AActor, public ISideQuestInteractable
{
    GENERATED_BODY()
public:
    AQuestInteractableActor();
    virtual FText GetInteractionLabel_Implementation() const override { return InteractionLabel; }
    virtual bool CanInteract_Implementation(APawn* Interactor) const override;
    virtual void Interact_Implementation(APawn* Interactor) override;

    /** Called by the player after the final dialogue line (or immediately for a silent interaction). */
    void CompleteInteraction();
    void Configure(const FText& Label, ESideQuestStep InRequiredStep, ESideQuestStep InResultStep,
        bool bInAdvanceQuest, const TArray<FSideQuestDialogueLine>& InDialogue);

    UFUNCTION(BlueprintImplementableEvent, Category = "Interaction|Presentation") void OnInteractionCompleted();
    UPROPERTY(BlueprintAssignable, Category = "Interaction") FInteractionFinished OnInteractionFinished;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<USceneComponent> SceneRoot;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction") TObjectPtr<USphereComponent> InteractionBounds;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction") FText InteractionLabel = NSLOCTEXT("Interaction", "Talk", "Talk");
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction") ESideQuestStep RequiredStep = ESideQuestStep::TalkToMildred;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction") ESideQuestStep ResultStep = ESideQuestStep::TalkToGuard;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction") bool bAdvanceQuest = true;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue") TArray<FSideQuestDialogueLine> Dialogue;
};
