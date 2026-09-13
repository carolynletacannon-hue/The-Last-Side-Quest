#include "Boss/MittensBoss.h"

#include "Combat/HealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/PlayerController.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Player/SideQuestCharacter.h"
#include "Quest/SideQuestGameState.h"
#include "UI/SideQuestHUD.h"

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
    EntranceTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    EntranceTrigger->SetGenerateOverlapEvents(true);
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
    EntranceTrigger->SetWorldLocation(GetActorLocation() + EntranceTriggerOffset);
    EntranceBlocker->SetWorldLocation(GetActorLocation() + EntranceBlockerOffset);
    EntranceTrigger->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
    EntranceBlocker->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
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
    ShowSubtitle(TEXT("Player"), TEXT("Oh, come on."), 3.0f, false);
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
    ++PounceCount;
    if (PounceCount == 1) ShowSubtitle(TEXT("Player"), TEXT("Bad cat!"));
    MoveStart = GetActorLocation();
    FVector Direction = (CombatTarget->GetActorLocation() - MoveStart).GetSafeNormal2D();
    MoveTarget = ClampToArena(MoveStart + Direction * FMath::Min(PounceRange, FVector::Dist2D(MoveStart, CombatTarget->GetActorLocation()) + 100.0f));
    MoveDuration = .32f; StateEndTime = GetWorld()->GetTimeSeconds() + MoveDuration; bDamageApplied = false; AttackState = EAttackState::Moving;
}

void AMittensBoss::BeginChaosDash()
{
    ++ChaosDashCount;
    if (ChaosDashCount == 1)
        ShowSubtitle(TEXT("Player"), TEXT("WHY ARE YOU SO FAST?!"));
    else if (ChaosDashCount == 2)
        ShowSubtitle(TEXT("Mittens"), TEXT("Mrrrp."));
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
    if (!bDefeated && !bLowHealthBarkPlayed && HealthComponent->GetHealthFraction() <= 0.3f)
    {
        bLowHealthBarkPlayed = true;
        ShowSubtitle(TEXT("Player"), TEXT("That's right. Fear me."), 2.5f, false);
    }
}

void AMittensBoss::ShowSubtitle(const TCHAR* Speaker, const TCHAR* Text, float Duration, bool bOnlyIfClear) const
{
    if (!GetWorld()) return;
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (ASideQuestHUD* HUD = PC->GetHUD<ASideQuestHUD>(); HUD && (!bOnlyIfClear || !HUD->HasGameplaySubtitle()))
            HUD->ShowGameplaySubtitle(FText::FromString(FString(Speaker)), FText::FromString(FString(Text)), Duration);
    }
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
        auto Line = [](const TCHAR* Speaker, const TCHAR* Text)
        {
            FSideQuestDialogueLine Result;
            Result.Speaker = FText::FromString(FString(Speaker));
            Result.Text = FText::FromString(FString(Text));
            return Result;
        };
        Player->BeginDialogue({
            Line(TEXT("Mittens"), TEXT("Meow.")),
            Line(TEXT("Player"), TEXT("That's it?")),
            Line(TEXT("Player"), TEXT("You tried to kill me. Do not purr at me.")),
            Line(TEXT("Mittens"), TEXT("Mrrrp.")),
            Line(TEXT("Player"), TEXT("I know what you're doing. You're lucky you're fluffy."))},
            nullptr, ESideQuestStep::PickUpMittens, ESideQuestStep::PickUpMittens, false);
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
        if (ASideQuestCharacter* Player = Cast<ASideQuestCharacter>(Interactor))
        {
            FSideQuestDialogueLine PlayerLine;
            PlayerLine.Speaker = FText::FromString(TEXT("Player"));
            PlayerLine.Text = FText::FromString(TEXT("Ow. Back legs. Right. Oh, NOW you're happy."));
            Player->BeginDialogue({PlayerLine}, nullptr,
                ESideQuestStep::ReturnToMildred, ESideQuestStep::ReturnToMildred, false);
        }
    }
}
