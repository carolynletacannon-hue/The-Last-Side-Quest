#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SideQuestGameMode.generated.h"

UCLASS()
class THELASTSIDEQUEST_API ASideQuestGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASideQuestGameMode();
protected:
    virtual void BeginPlay() override;
};
