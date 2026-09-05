#include "Boss/MittensBoss.h"

#include "Combat/HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SideQuestCharacter.h"
#include "Quest/SideQuestGameState.h"

AMittensBoss::AMittensBoss()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
    HealthComponent->SetMaxHealth(300.0f);
    GetCapsuleComponent()->InitCapsuleSize(34.0f, 36.0f);
    GetCharacterMovement()->MaxWalkSpeed = 380.0f;
    GetCharacterMovement()->bOrientRotationToMovement = true;

    PlaceholderCatBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaceholderCatBody"));
    PlaceholderCatBody->SetupAttachment(RootComponent);
    PlaceholderCatBody->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere")));
    PlaceholderCatBody->SetRelativeScale3D({.55f, .3f, .3f});
    PlaceholderCatBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    EntranceTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BossEntranceTrigger"));
    EntranceTrigger->SetupAttachment(RootComponent);
    EntranceTrigger->SetBoxExtent({100, 650, 250});
    EntranceTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    EntranceTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    EntranceBlocker = CreateDefaultSubobject<UBoxComponent>(TEXT("BossEntranceBlocker"));
    EntranceBlocker->SetupAttachment(RootComponent);
    EntranceBlocker->SetBoxExtent({60, 1300, 260});
    EntranceBlocker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EntranceBlocker->SetCollisionResponseToAllChannels(ECR_Block);
    EntranceVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossEntranceVisual"));
    EntranceVisual->SetupAttachment(EntranceBlocker);
    EntranceVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EntranceVisual->SetVisibility(false);
}

void AMittensBoss::BeginPlay()
{
    Super::BeginPlay();
    ArenaCenter = GetActorLocation() + ArenaCenterOffset;
    EntranceTrigger->SetRelativeLocation(EntranceTriggerOffset);
    EntranceBlocker->SetRelativeLocation(EntranceBlockerOffset);
    EntranceTrigger->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleEntranceOverlap);
    HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::HandleHealthChanged);
    HealthComponent->OnDeath.AddDynamic(this, &ThisClass::HandleDefeat);
}

float AMittensBoss::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    return IsEncounterActive() ? Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser) : 0.0f;
}

void AMittensBoss::HandleEntranceOverlap(UPrimitiveComponent*, AActor* Other, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    if (APawn* Player = Cast<ASideQuestCharacter>(Other)) BeginEncounter(Player);
}

void AMittensBoss::BeginEncounter(APawn* Player)
{
    ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>();
    if (bEncounterActive || bDefeated || !State || !State->TryAdvanceQuest(ESideQuestStep::ReachMittens, ESideQuestStep::FightMittens)) return;
    bEncounterActive = true;
    CombatTarget = Player;
    EntranceTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EntranceBlocker->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    EntranceVisual->SetVisibility(true, true);
    PlayEncounterStartPresentation();
    StartBossMusic();
    PlayMeowPresentation();
    NextAttackTime = GetWorld()->GetTimeSeconds() + 1.5f;
}

void AMittensBoss::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!IsEncounterActive() || !IsValid(CombatTarget)) return;
    const UHealthComponent* TargetHealth = CombatTarget->FindComponentByClass<UHealthComponent>();
    if (TargetHealth && TargetHealth->IsDead()) { GetCharacterMovement()->StopMovementImmediately(); return; }

    const float Now = GetWorld()->GetTimeSeconds();
    if (AttackState == EAttackState::Idle)
    {
        const FVector ToTarget = CombatTarget->GetActorLocation() - GetActorLocation();
        if (!ToTarget.IsNearlyZero()) SetActorRotation(FRotator(0, ToTarget.Rotation().Yaw, 0));
        if (Now >= NextAttackTime) ChooseAttack();
        else if (FVector::Dist2D(GetActorLocation(), CombatTarget->GetActorLocation()) > ScratchRange * .8f)
            AddMovementInput(ToTarget.GetSafeNormal2D());
    }
    else if (AttackState == EAttackState::Telegraph && Now >= StateEndTime)
    {
        if (ScratchBeat < 0) BeginPounce(); else if (ScratchBeat == 99) BeginChaosDash(); else BeginScratch();
    }
    else if (AttackState == EAttackState::Moving)
    {
        const float Alpha = MoveDuration > 0 ? 1.0f - FMath::Max(0.0f, StateEndTime - Now) / MoveDuration : 1.0f;
        SetActorLocation(ClampToArena(FMath::Lerp(MoveStart, MoveTarget, FMath::Clamp(Alpha, 0.0f, 1.0f))), true);
        DamageTargetOnce(ScratchBeat == 99 ? ChaosDashDamage : PounceDamage, AttackHitRadius);
        if (Now >= StateEndTime) { if (ScratchBeat != 99) PlayPounceImpact(); FinishAttack(ScratchBeat == 99 ? .8f : 1.0f); }
    }
    else if (AttackState == EAttackState::Scratch && Now >= StateEndTime)
    {
        ++ScratchBeat;
        PlayScratchPresentation(ScratchBeat);
        DamageTargetOnce(ScratchDamagePerBeat, ScratchRange);
        bDamageApplied = false;
        if (ScratchBeat >= 3) FinishAttack(.8f); else StateEndTime = Now + .22f;
    }
    else if (AttackState == EAttackState::Recovery && Now >= StateEndTime)
    {
        AttackState = EAttackState::Idle;
        NextAttackTime = Now + AttackCooldown;
    }
}

void AMittensBoss::ChooseAttack()
{
    GetCharacterMovement()->StopMovementImmediately();
    const float Distance = FVector::Dist2D(GetActorLocation(), CombatTarget->GetActorLocation());
    if (Distance <= ScratchRange) { ScratchBeat = 0; StateEndTime = GetWorld()->GetTimeSeconds() + .45f; }
    else if (FMath::RandBool()) { ScratchBeat = -1; StateEndTime = GetWorld()->GetTimeSeconds() + .75f; PlayPounceTelegraph(); }
    else { ScratchBeat = 99; StateEndTime = GetWorld()->GetTimeSeconds() + .6f; PlayChaosDashPresentation(); }
    AttackState = EAttackState::Telegraph;
}

void AMittensBoss::BeginPounce()
{
    MoveStart = GetActorLocation();
    FVector Direction = (CombatTarget->GetActorLocation() - MoveStart).GetSafeNormal2D();
    MoveTarget = ClampToArena(MoveStart + Direction * FMath::Min(PounceRange, FVector::Dist2D(MoveStart, CombatTarget->GetActorLocation()) + 100.0f));
    MoveDuration = .32f; StateEndTime = GetWorld()->GetTimeSeconds() + MoveDuration; bDamageApplied = false; AttackState = EAttackState::Moving;
}

void AMittensBoss::BeginChaosDash()
{
    MoveStart = GetActorLocation();
    const FVector Across = (MoveStart - ArenaCenter).GetSafeNormal2D();
    const FVector Direction = Across.IsNearlyZero() ? GetActorRightVector() : -Across;
    MoveTarget = ClampToArena(ArenaCenter + Direction * ArenaRadius * .88f);
    MoveDuration = .55f; StateEndTime = GetWorld()->GetTimeSeconds() + MoveDuration; bDamageApplied = false; AttackState = EAttackState::Moving;
}

void AMittensBoss::BeginScratch()
{
    AttackState = EAttackState::Scratch; ScratchBeat = 0; bDamageApplied = false;
    StateEndTime = GetWorld()->GetTimeSeconds();
}

void AMittensBoss::FinishAttack(float RecoverySeconds)
{
    AttackState = EAttackState::Recovery; StateEndTime = GetWorld()->GetTimeSeconds() + RecoverySeconds; bDamageApplied = false;
}

void AMittensBoss::DamageTargetOnce(float Damage, float Radius)
{
    if (bDamageApplied || !IsEncounterActive() || FVector::Dist2D(GetActorLocation(), CombatTarget->GetActorLocation()) > Radius) return;
    bDamageApplied = true;
    UGameplayStatics::ApplyDamage(CombatTarget, Damage, GetController(), this, UDamageType::StaticClass());
}

FVector AMittensBoss::ClampToArena(const FVector& Location) const
{
    FVector Offset = Location - ArenaCenter; Offset.Z = 0;
    return ArenaCenter + Offset.GetClampedToMaxSize2D(ArenaRadius) + FVector(0, 0, GetActorLocation().Z - ArenaCenter.Z);
}

void AMittensBoss::HandleHealthChanged(UHealthComponent*, float, float Delta, AActor* DamageCauser)
{
    if (!bDefeated && Delta < 0) { PlayHitPresentation(); if (!CombatTarget) CombatTarget = Cast<APawn>(DamageCauser); }
}

void AMittensBoss::HandleDefeat(UHealthComponent*, AActor*)
{
    if (bDefeated) return;
    bDefeated = true; bEncounterActive = false; AttackState = EAttackState::Idle;
    GetCharacterMovement()->DisableMovement(); EntranceBlocker->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    EntranceVisual->SetVisibility(false, true); StopBossMusic(); PlayDefeatPresentation();
    if (ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>())
        State->TryAdvanceQuest(ESideQuestStep::FightMittens, ESideQuestStep::PickUpMittens);
    if (ASideQuestCharacter* Player = Cast<ASideQuestCharacter>(CombatTarget))
    {
        FSideQuestDialogueLine Meow; Meow.Speaker = FText::FromString(TEXT("Mittens")); Meow.Text = FText::FromString(TEXT("Meow."));
        Player->BeginDialogue({Meow}, nullptr, ESideQuestStep::PickUpMittens, ESideQuestStep::PickUpMittens, false);
    }
}

FText AMittensBoss::GetInteractionLabel_Implementation() const { return FText::FromString(TEXT("Pick up Mittens")); }

bool AMittensBoss::CanInteract_Implementation(APawn*) const
{
    const ASideQuestGameState* State = GetWorld() ? GetWorld()->GetGameState<ASideQuestGameState>() : nullptr;
    return bDefeated && State && State->GetQuestStep() == ESideQuestStep::PickUpMittens;
}

void AMittensBoss::Interact_Implementation(APawn* Interactor)
{
    if (!CanInteract_Implementation(Interactor)) return;
    ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>();
    if (State && State->TryAdvanceQuest(ESideQuestStep::PickUpMittens, ESideQuestStep::ReturnToMildred))
    {
        PlayPickupPresentation(Interactor); SetActorHiddenInGame(true); SetActorEnableCollision(false); SetActorTickEnabled(false);
    }
}
