#include "SideQuestGameMode.h"

#include "Player/SideQuestCharacter.h"
#include "UI/SideQuestHUD.h"
#include "Quest/SideQuestGameState.h"

ASideQuestGameMode::ASideQuestGameMode()
{
    DefaultPawnClass = ASideQuestCharacter::StaticClass();
    HUDClass = ASideQuestHUD::StaticClass();
    GameStateClass = ASideQuestGameState::StaticClass();
}
