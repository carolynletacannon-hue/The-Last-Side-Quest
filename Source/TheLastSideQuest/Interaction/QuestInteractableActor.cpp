#include "Interaction/QuestInteractableActor.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Player/SideQuestCharacter.h"
#include "Quest/SideQuestGameState.h"

AQuestInteractableActor::AQuestInteractableActor()
{
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);
    InteractionBounds = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionBounds"));
    InteractionBounds->SetupAttachment(SceneRoot);
    InteractionBounds->InitSphereRadius(55.0f);
    InteractionBounds->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionBounds->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionBounds->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

bool AQuestInteractableActor::CanInteract_Implementation(APawn* Interactor) const
{
    const ASideQuestGameState* State = GetWorld() ? GetWorld()->GetGameState<ASideQuestGameState>() : nullptr;
    return State && State->GetQuestStep() == RequiredStep;
}

void AQuestInteractableActor::Interact_Implementation(APawn* Interactor)
{
    if (!CanInteract_Implementation(Interactor)) return;
    if (ASideQuestCharacter* Player = Cast<ASideQuestCharacter>(Interactor))
    {
        Player->BeginDialogue(Dialogue, this, RequiredStep, ResultStep, bAdvanceQuest);
    }
}

void AQuestInteractableActor::CompleteInteraction()
{
    OnInteractionCompleted();
    OnInteractionFinished.Broadcast();
}

void AQuestInteractableActor::Configure(const FText& Label, ESideQuestStep InRequiredStep,
    ESideQuestStep InResultStep, bool bInAdvanceQuest, const TArray<FSideQuestDialogueLine>& InDialogue)
{
    InteractionLabel = Label;
    RequiredStep = InRequiredStep;
    ResultStep = InResultStep;
    bAdvanceQuest = bInAdvanceQuest;
    Dialogue = InDialogue;
}
