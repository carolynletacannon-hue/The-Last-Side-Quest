#include "Quest/QuestProgressTrigger.h"
#include "Player/SideQuestCharacter.h"
#include "GameFramework/PlayerController.h"
#include "Quest/SideQuestGameState.h"
#include "UI/SideQuestHUD.h"

AQuestProgressTrigger::AQuestProgressTrigger() { OnActorBeginOverlap.AddDynamic(this, &ThisClass::HandleOverlap); }
void AQuestProgressTrigger::Configure(ESideQuestStep InRequiredStep, ESideQuestStep InResultStep,
    const FText& InSubtitleSpeaker, const FText& InSubtitleText)
{
    RequiredStep = InRequiredStep;
    ResultStep = InResultStep;
    SubtitleSpeaker = InSubtitleSpeaker;
    SubtitleText = InSubtitleText;
}
void AQuestProgressTrigger::HandleOverlap(AActor*, AActor* OtherActor)
{
    if (!Cast<ASideQuestCharacter>(OtherActor)) return;
    if (ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>())
    {
        if (State->TryAdvanceQuest(RequiredStep, ResultStep))
        {
            if (!SubtitleText.IsEmpty())
            {
                if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
                    if (ASideQuestHUD* HUD = PC->GetHUD<ASideQuestHUD>())
                        HUD->ShowGameplaySubtitle(SubtitleSpeaker, SubtitleText, 4.0f);
            }
            SetActorEnableCollision(false);
        }
    }
}
