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
    return State && (State->GetQuestStep() == RequiredStep ||
        (bHandlesMildredEnding && State->GetQuestStep() == ESideQuestStep::ReturnToMildred));
}

void AQuestInteractableActor::Interact_Implementation(APawn* Interactor)
{
    if (!CanInteract_Implementation(Interactor)) return;
    if (ASideQuestCharacter* Player = Cast<ASideQuestCharacter>(Interactor))
    {
        ActiveInteractor = Interactor;
        const bool bReturningMittens = bHandlesMildredEnding &&
            GetWorld()->GetGameState<ASideQuestGameState>()->GetQuestStep() == ESideQuestStep::ReturnToMildred;
        if (bReturningMittens) PlayReturnMittensPresentation();
        Player->BeginDialogue(bReturningMittens ? ReturnDialogue : Dialogue, this,
            bReturningMittens ? ESideQuestStep::ReturnToMildred : RequiredStep,
            bReturningMittens ? ESideQuestStep::Complete : ResultStep,
            bReturningMittens ? false : bAdvanceQuest);
    }
}

void AQuestInteractableActor::CompleteInteraction()
{
    OnInteractionCompleted();
    OnInteractionFinished.Broadcast();
    if (bHandlesMildredEnding)
    {
        if (ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>())
            State->BeginEndingSequence(Cast<ASideQuestCharacter>(ActiveInteractor), this);
    }
    ActiveInteractor = nullptr;
}

void AQuestInteractableActor::ConfigureEnding(const TArray<FSideQuestDialogueLine>& InReturnDialogue,
    const TArray<FSideQuestDialogueLine>& InFinalDialogue)
{
    bHandlesMildredEnding = true;
    ReturnDialogue = InReturnDialogue;
    FinalDialogue = InFinalDialogue;
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
