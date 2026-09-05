#include "Enemy/SideQuestEnemyAIController.h"

#include "Combat/HealthComponent.h"
#include "Enemy/SideQuestEnemy.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

ASideQuestEnemyAIController::ASideQuestEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 0.1f;
}

void ASideQuestEnemyAIController::SetCombatTarget(APawn* NewTarget)
{
    if (IsValid(NewTarget))
    {
        CombatTarget = NewTarget;
        LastTargetSeenTime = GetWorld()->GetTimeSeconds();
    }
}

void ASideQuestEnemyAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    ASideQuestEnemy* Enemy = Cast<ASideQuestEnemy>(GetPawn());
    APawn* Target = CombatTarget.Get();
    if (!Enemy || !Target || Enemy->GetHealthComponent()->IsDead())
    {
        StopMovement();
        return;
    }

    const UHealthComponent* TargetHealth = Target->FindComponentByClass<UHealthComponent>();
    if ((TargetHealth && TargetHealth->IsDead()) ||
        GetWorld()->GetTimeSeconds() - LastTargetSeenTime > Enemy->GetTargetMemoryDuration())
    {
        CombatTarget.Reset();
        StopMovement();
        return;
    }

    const float Distance = FVector::Dist2D(Enemy->GetActorLocation(), Target->GetActorLocation());
    if (Distance > Enemy->GetAttackRange())
    {
        MoveToActor(Target, Enemy->GetAttackRange() * 0.75f, true, true, true, nullptr, true);
        return;
    }

    StopMovement();
    const FVector TargetDirection = Target->GetActorLocation() - Enemy->GetActorLocation();
    Enemy->SetActorRotation(FRotator(0.0f, TargetDirection.Rotation().Yaw, 0.0f));

    const float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime >= NextAttackTime)
    {
        NextAttackTime = CurrentTime + Enemy->GetAttackCooldown();
        Enemy->PlayAttackPresentation();
        UGameplayStatics::ApplyDamage(Target, Enemy->GetAttackDamage(), this, Enemy, UDamageType::StaticClass());
    }
}
