#include "Quest/QuestProgressTrigger.h"
#include "Player/SideQuestCharacter.h"
#include "Quest/SideQuestGameState.h"

AQuestProgressTrigger::AQuestProgressTrigger() { OnActorBeginOverlap.AddDynamic(this, &ThisClass::HandleOverlap); }
void AQuestProgressTrigger::HandleOverlap(AActor*, AActor* OtherActor)
{
    if (!Cast<ASideQuestCharacter>(OtherActor)) return;
    if (ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>())
    {
        if (State->TryAdvanceQuest(RequiredStep, ResultStep)) SetActorEnableCollision(false);
    }
}
