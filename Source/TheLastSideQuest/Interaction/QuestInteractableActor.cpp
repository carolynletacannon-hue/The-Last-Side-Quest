#include "Interaction/QuestInteractableActor.h"
#include "Components/SceneComponent.h"
#include "Player/SideQuestCharacter.h"
#include "Quest/SideQuestGameState.h"

AQuestInteractableActor::AQuestInteractableActor()
{
    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);
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
