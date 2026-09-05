#include "SideQuestGameMode.h"

#include "Player/SideQuestCharacter.h"
#include "UI/SideQuestHUD.h"
#include "Quest/SideQuestGameState.h"
#include "World/Phase4World.h"
#include "EngineUtils.h"

ASideQuestGameMode::ASideQuestGameMode()
{
    DefaultPawnClass = ASideQuestCharacter::StaticClass();
    HUDClass = ASideQuestHUD::StaticClass();
    GameStateClass = ASideQuestGameState::StaticClass();
}

void ASideQuestGameMode::BeginPlay()
{
    Super::BeginPlay();
    for (TActorIterator<APhase4World> It(GetWorld()); It; ++It) return;
    GetWorld()->SpawnActor<APhase4World>();
}
