#include "Quest/SideQuestGameState.h"

ASideQuestGameState::ASideQuestGameState()
{
    ObjectiveText.Add(ESideQuestStep::TalkToMildred, NSLOCTEXT("Quest", "Mildred", "Talk to Mildred"));
    ObjectiveText.Add(ESideQuestStep::TalkToGuard, NSLOCTEXT("Quest", "Guard", "Ask the guard about Mittens"));
    ObjectiveText.Add(ESideQuestStep::FollowForestClue, NSLOCTEXT("Quest", "Forest", "Follow the clues into the forest"));
    ObjectiveText.Add(ESideQuestStep::TalkToGoblin, NSLOCTEXT("Quest", "Goblin", "Talk to the terrified goblin"));
    ObjectiveText.Add(ESideQuestStep::EnterRuins, NSLOCTEXT("Quest", "Ruins", "Follow Mittens' trail into the ruins"));
    ObjectiveText.Add(ESideQuestStep::OpenRuinsGate, NSLOCTEXT("Quest", "Gate", "Read the inscription and open the gate"));
    ObjectiveText.Add(ESideQuestStep::ReachMittens, NSLOCTEXT("Quest", "ReachMittens", "Enter the chamber beyond"));
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
    if (QuestStep != ExpectedStep || static_cast<uint8>(NewStep) < static_cast<uint8>(QuestStep)) return false;
    QuestStep = NewStep;
    OnQuestStepChanged.Broadcast(QuestStep);
    return true;
}
