#include "Quest/SideQuestGameState.h"
#include "Interaction/QuestInteractableActor.h"
#include "Player/SideQuestCharacter.h"
#include "TimerManager.h"

ASideQuestGameState::ASideQuestGameState()
{
    ObjectiveText.Add(ESideQuestStep::TalkToMildred, NSLOCTEXT("Quest", "Mildred", "Talk to Mildred"));
    ObjectiveText.Add(ESideQuestStep::TalkToGuard, NSLOCTEXT("Quest", "Guard", "Ask the guard about Mittens"));
    ObjectiveText.Add(ESideQuestStep::FollowForestClue, NSLOCTEXT("Quest", "Forest", "Follow the clues into the forest"));
    ObjectiveText.Add(ESideQuestStep::TalkToGoblin, NSLOCTEXT("Quest", "Goblin", "Talk to the terrified goblin"));
    ObjectiveText.Add(ESideQuestStep::EnterRuins, NSLOCTEXT("Quest", "Ruins", "Follow Mittens' trail into the ruins"));
    ObjectiveText.Add(ESideQuestStep::OpenRuinsGate, NSLOCTEXT("Quest", "Gate", "Read the inscription and open the gate"));
    ObjectiveText.Add(ESideQuestStep::ReachMittens, NSLOCTEXT("Quest", "ReachMittens", "Enter the chamber beyond"));
    ObjectiveText.Add(ESideQuestStep::FightMittens, NSLOCTEXT("Quest", "FightMittens", "Defeat Mittens, Devourer of Worlds"));
    ObjectiveText.Add(ESideQuestStep::PickUpMittens, NSLOCTEXT("Quest", "Pickup", "Pick up Mittens"));
    ObjectiveText.Add(ESideQuestStep::ReturnToMildred, NSLOCTEXT("Quest", "Return", "Return Mittens to Mildred"));
    ObjectiveText.Add(ESideQuestStep::Complete, NSLOCTEXT("Quest", "Complete", "Quest complete — Reward: 3 Gold"));
}

FText ASideQuestGameState::GetObjectiveText() const
{
    if (const FText* Text = ObjectiveText.Find(QuestStep)) return *Text;
    return FText::GetEmpty();
}

bool ASideQuestGameState::TryAdvanceQuest(ESideQuestStep ExpectedStep, ESideQuestStep NewStep)
{
    if (QuestStep != ExpectedStep || NewStep != static_cast<ESideQuestStep>(static_cast<uint8>(ExpectedStep) + 1)) return false;
    QuestStep = NewStep;
    OnQuestStepChanged.Broadcast(QuestStep);
    PlayQuestStepPresentation(QuestStep);
    return true;
}

bool ASideQuestGameState::BeginEndingSequence(ASideQuestCharacter* Player, AQuestInteractableActor* Mildred)
{
    if (!Player || !Mildred || EndingState != EEndingPresentationState::None ||
        !TryAdvanceQuest(ESideQuestStep::ReturnToMildred, ESideQuestStep::Complete))
    {
        return false;
    }

    EndingPlayer = Player;
    EndingMildred = Mildred;
    SetEndingState(EEndingPresentationState::QuestComplete);
    Mildred->PlayQuestCompletePresentation();
    GetWorldTimerManager().SetTimer(EndingTimer, this, &ThisClass::BeginFinalDialogue, 2.75f, false);
    return true;
}

void ASideQuestGameState::BeginFinalDialogue()
{
    if (EndingState != EEndingPresentationState::QuestComplete || !IsValid(EndingPlayer) || !IsValid(EndingMildred)) return;
    SetEndingState(EEndingPresentationState::FinalDialogue);
    EndingMildred->PlayFinalJokePresentation();
    EndingPlayer->BeginEndingDialogue(EndingMildred->GetFinalDialogue());
}

void ASideQuestGameState::FinishFinalDialogue()
{
    if (EndingState != EEndingPresentationState::FinalDialogue) return;
    SetEndingState(EEndingPresentationState::Fading);
    FadeStartedAt = GetWorld()->GetTimeSeconds();
    if (EndingMildred) EndingMildred->PlayEndingTransitionPresentation();
    GetWorldTimerManager().SetTimer(EndingTimer, this, &ThisClass::ShowCredits, 1.25f, false);
}

void ASideQuestGameState::ShowCredits()
{
    if (EndingState == EEndingPresentationState::Fading) SetEndingState(EEndingPresentationState::Credits);
}

void ASideQuestGameState::SetEndingState(EEndingPresentationState NewState)
{
    EndingState = NewState;
}

float ASideQuestGameState::GetFadeOpacity() const
{
    if (EndingState == EEndingPresentationState::Credits) return 1.0f;
    if (EndingState != EEndingPresentationState::Fading || !GetWorld()) return 0.0f;
    return FMath::Clamp((GetWorld()->GetTimeSeconds() - FadeStartedAt) / 1.25f, 0.0f, 1.0f);
}
