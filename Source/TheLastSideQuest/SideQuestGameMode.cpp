#include "SideQuestGameMode.h"

#include "Player/SideQuestCharacter.h"
#include "UI/SideQuestHUD.h"

ASideQuestGameMode::ASideQuestGameMode()
{
    DefaultPawnClass = ASideQuestCharacter::StaticClass();
    HUDClass = ASideQuestHUD::StaticClass();
}
