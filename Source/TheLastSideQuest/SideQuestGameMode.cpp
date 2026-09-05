#include "SideQuestGameMode.h"

#include "Player/SideQuestCharacter.h"

ASideQuestGameMode::ASideQuestGameMode()
{
    DefaultPawnClass = ASideQuestCharacter::StaticClass();
}
