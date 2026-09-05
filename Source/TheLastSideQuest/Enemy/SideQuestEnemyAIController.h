#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SideQuestEnemyAIController.generated.h"

/** Lightweight chase/attack controller for all normal enemy variants. */
UCLASS()
class THELASTSIDEQUEST_API ASideQuestEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    ASideQuestEnemyAIController();
    virtual void Tick(float DeltaSeconds) override;

    void SetCombatTarget(APawn* NewTarget);

private:
    TWeakObjectPtr<APawn> CombatTarget;
    float LastTargetSeenTime = 0.0f;
    float NextAttackTime = 0.0f;
    bool bUseDirectMoveFallback = false;
};
