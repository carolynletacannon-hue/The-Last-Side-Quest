#include "Enemy/SideQuestEnemy.h"

#include "Combat/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/SideQuestEnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"

ASideQuestEnemy::ASideQuestEnemy()
{
    PrimaryActorTick.bCanEverTick = false;
    AIControllerClass = ASideQuestEnemyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
    PawnSensing->SightRadius = 1200.0f;
    PawnSensing->SetPeripheralVisionAngle(70.0f);
    PawnSensing->SensingInterval = 0.25f;
    PawnSensing->bOnlySensePlayers = true;

    GetCharacterMovement()->MaxWalkSpeed = 300.0f;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 420.0f, 0.0f);
}

void ASideQuestEnemy::BeginPlay()
{
    Super::BeginPlay();
    PawnSensing->OnSeePawn.AddDynamic(this, &ThisClass::HandlePawnSeen);
    HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::HandleHealthChanged);
    HealthComponent->OnDeath.AddDynamic(this, &ThisClass::HandleDeath);
}

void ASideQuestEnemy::HandlePawnSeen(APawn* SeenPawn)
{
    if (!HealthComponent->IsDead())
    {
        if (ASideQuestEnemyAIController* EnemyController = Cast<ASideQuestEnemyAIController>(GetController()))
        {
            EnemyController->SetCombatTarget(SeenPawn);
        }
    }
}

void ASideQuestEnemy::HandleHealthChanged(UHealthComponent* Component, float NewHealth, float HealthDelta, AActor* DamageCauser)
{
    PlayHitPresentation();

    if (APawn* AttackingPawn = DamageCauser ? Cast<APawn>(DamageCauser) : nullptr)
    {
        if (ASideQuestEnemyAIController* EnemyController = Cast<ASideQuestEnemyAIController>(GetController()))
        {
            EnemyController->SetCombatTarget(AttackingPawn);
        }
    }
}

void ASideQuestEnemy::HandleDeath(UHealthComponent* Component, AActor* DamageCauser)
{
    PlayDeathPresentation();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCharacterMovement()->DisableMovement();

    if (AController* EnemyController = GetController())
    {
        EnemyController->StopMovement();
        EnemyController->UnPossess();
    }

    SetLifeSpan(DestroyDelay);
}
