#include "Player/SideQuestCharacter.h"

#include "Camera/CameraComponent.h"
#include "Combat/HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interaction/QuestInteractableActor.h"
#include "Interaction/SideQuestInteractable.h"
#include "Quest/SideQuestGameState.h"
#include "TimerManager.h"
#include "UI/SideQuestHUD.h"

ASideQuestCharacter::ASideQuestCharacter()
{
    GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 500.0f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.0f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->SocketOffset = FVector(0.0f, 45.0f, 65.0f);
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->bEnableCameraLag = true;
    CameraBoom->CameraLagSpeed = 12.0f;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

    SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
    SwordMesh->SetupAttachment(GetMesh(), BackSwordSocket);
    SwordMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    DefaultMappingContext = CreateDefaultSubobject<UInputMappingContext>(TEXT("DefaultMappingContext"));
    MoveForwardAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveForwardAction"));
    MoveRightAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveRightAction"));
    LookYawAction = CreateDefaultSubobject<UInputAction>(TEXT("LookYawAction"));
    LookPitchAction = CreateDefaultSubobject<UInputAction>(TEXT("LookPitchAction"));
    JumpAction = CreateDefaultSubobject<UInputAction>(TEXT("JumpAction"));
    AttackAction = CreateDefaultSubobject<UInputAction>(TEXT("AttackAction"));
    HeavyAttackAction = CreateDefaultSubobject<UInputAction>(TEXT("HeavyAttackAction"));
    DodgeAction = CreateDefaultSubobject<UInputAction>(TEXT("DodgeAction"));
    RestartAction = CreateDefaultSubobject<UInputAction>(TEXT("RestartAction"));
    InteractAction = CreateDefaultSubobject<UInputAction>(TEXT("InteractAction"));
    QuitAction = CreateDefaultSubobject<UInputAction>(TEXT("QuitAction"));

    MoveForwardAction->ValueType = EInputActionValueType::Axis1D;
    MoveRightAction->ValueType = EInputActionValueType::Axis1D;
    LookYawAction->ValueType = EInputActionValueType::Axis1D;
    LookPitchAction->ValueType = EInputActionValueType::Axis1D;
    JumpAction->ValueType = EInputActionValueType::Boolean;
    AttackAction->ValueType = EInputActionValueType::Boolean;
    HeavyAttackAction->ValueType = EInputActionValueType::Boolean;
    DodgeAction->ValueType = EInputActionValueType::Boolean;
    RestartAction->ValueType = EInputActionValueType::Boolean;
    InteractAction->ValueType = EInputActionValueType::Boolean;
    QuitAction->ValueType = EInputActionValueType::Boolean;

    DefaultMappingContext->MapKey(MoveForwardAction, EKeys::W);
    DefaultMappingContext->MapKey(MoveForwardAction, EKeys::Gamepad_LeftY);
    DefaultMappingContext->MapKey(MoveForwardAction, EKeys::S).Modifiers.Add(
        CreateDefaultSubobject<UInputModifierNegate>(TEXT("NegateMoveBackward")));
    DefaultMappingContext->MapKey(MoveRightAction, EKeys::D);
    DefaultMappingContext->MapKey(MoveRightAction, EKeys::Gamepad_LeftX);
    DefaultMappingContext->MapKey(MoveRightAction, EKeys::A).Modifiers.Add(
        CreateDefaultSubobject<UInputModifierNegate>(TEXT("NegateMoveLeft")));
    DefaultMappingContext->MapKey(LookYawAction, EKeys::MouseX);
    DefaultMappingContext->MapKey(LookYawAction, EKeys::Gamepad_RightX);
    DefaultMappingContext->MapKey(LookPitchAction, EKeys::MouseY).Modifiers.Add(
        CreateDefaultSubobject<UInputModifierNegate>(TEXT("InvertMousePitch")));
    DefaultMappingContext->MapKey(LookPitchAction, EKeys::Gamepad_RightY);
    DefaultMappingContext->MapKey(JumpAction, EKeys::SpaceBar);
    DefaultMappingContext->MapKey(JumpAction, EKeys::Gamepad_FaceButton_Bottom);
    DefaultMappingContext->MapKey(AttackAction, EKeys::LeftMouseButton);
    DefaultMappingContext->MapKey(AttackAction, EKeys::Gamepad_RightTrigger);
    DefaultMappingContext->MapKey(HeavyAttackAction, EKeys::RightMouseButton);
    DefaultMappingContext->MapKey(HeavyAttackAction, EKeys::Gamepad_LeftTrigger);
    DefaultMappingContext->MapKey(DodgeAction, EKeys::LeftShift);
    DefaultMappingContext->MapKey(DodgeAction, EKeys::Gamepad_FaceButton_Right);
    DefaultMappingContext->MapKey(RestartAction, EKeys::R);
    DefaultMappingContext->MapKey(RestartAction, EKeys::Gamepad_Special_Right);
    DefaultMappingContext->MapKey(InteractAction, EKeys::E);
    DefaultMappingContext->MapKey(InteractAction, EKeys::Gamepad_FaceButton_Left);
    DefaultMappingContext->MapKey(QuitAction, EKeys::Escape);
    DefaultMappingContext->MapKey(QuitAction, EKeys::Gamepad_FaceButton_Right);
}

float ASideQuestCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    return (IsGameplayLocked() || IsDodging())
        ? 0.0f
        : Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASideQuestCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (!UGameplayStatics::GetActorOfClass(this, APlayerStart::StaticClass()) &&
        GetActorLocation().IsNearlyZero(1.0f))
    {
        SetActorLocation(FVector(0.0f, 0.0f, 80.0f), false, nullptr, ETeleportType::TeleportPhysics);
    }

    HealthComponent->OnHealthChanged.AddDynamic(this, &ThisClass::HandleHealthChanged);
    HealthComponent->OnDeath.AddDynamic(this, &ThisClass::HandleDeath);
    AttachSwordToSocket(BackSwordSocket);

    if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
    GetWorldTimerManager().SetTimer(InteractionScanTimer, this, &ThisClass::RefreshNearbyInteractable, 0.1f, true, 0.0f);
}

void ASideQuestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
    EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ThisClass::MoveForward);
    EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
    EnhancedInput->BindAction(LookYawAction, ETriggerEvent::Triggered, this, &ThisClass::LookYaw);
    EnhancedInput->BindAction(LookPitchAction, ETriggerEvent::Triggered, this, &ThisClass::LookPitch);
    EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::StartJump);
    EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
    EnhancedInput->BindAction(AttackAction, ETriggerEvent::Started, this, &ThisClass::LightAttack);
    EnhancedInput->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ThisClass::HeavyAttack);
    EnhancedInput->BindAction(DodgeAction, ETriggerEvent::Started, this, &ThisClass::Dodge);
    EnhancedInput->BindAction(RestartAction, ETriggerEvent::Started, this, &ThisClass::RestartAfterDeath);
    EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::Interact);
    EnhancedInput->BindAction(QuitAction, ETriggerEvent::Started, this, &ThisClass::QuitFromCredits);
}

void ASideQuestCharacter::RefreshNearbyInteractable()
{
    if (IsDead() || IsInDialogue() || IsGameplayLocked()) { CurrentInteractable = nullptr; return; }
    CurrentInteractable = nullptr;
    TArray<FOverlapResult> Results;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(InteractionScan), false, this);
    if (!GetWorld()->OverlapMultiByObjectType(Results, GetActorLocation(), FQuat::Identity,
        FCollisionObjectQueryParams::AllObjects, FCollisionShape::MakeSphere(InteractionRadius), Params)) return;
    float BestDistanceSq = TNumericLimits<float>::Max();
    for (const FOverlapResult& Result : Results)
    {
        AActor* Candidate = Result.GetActor();
        if (Candidate && Candidate->Implements<USideQuestInteractable>() &&
            ISideQuestInteractable::Execute_CanInteract(Candidate, this))
        {
            const float DistanceSq = FVector::DistSquared(GetActorLocation(), Candidate->GetActorLocation());
            if (DistanceSq < BestDistanceSq) { BestDistanceSq = DistanceSq; CurrentInteractable = Candidate; }
        }
    }
}

void ASideQuestCharacter::Interact()
{
    if (IsDead()) return;
    if (IsInDialogue())
    {
        if (GetWorld()->GetTimeSeconds() < NextDialogueAdvanceTime) return;
        ++DialogueIndex;
        NextDialogueAdvanceTime = GetWorld()->GetTimeSeconds() + 0.15f;
        if (!ActiveDialogue.IsValidIndex(DialogueIndex)) FinishDialogue();
        return;
    }
    if (IsValid(CurrentInteractable)) ISideQuestInteractable::Execute_Interact(CurrentInteractable, this);
}

FText ASideQuestCharacter::GetCurrentInteractionLabel() const
{
    return IsValid(CurrentInteractable) ? ISideQuestInteractable::Execute_GetInteractionLabel(CurrentInteractable) : FText::GetEmpty();
}
FText ASideQuestCharacter::GetDialogueSpeaker() const { return ActiveDialogue.IsValidIndex(DialogueIndex) ? ActiveDialogue[DialogueIndex].Speaker : FText::GetEmpty(); }
FText ASideQuestCharacter::GetDialogueText() const { return ActiveDialogue.IsValidIndex(DialogueIndex) ? ActiveDialogue[DialogueIndex].Text : FText::GetEmpty(); }

void ASideQuestCharacter::BeginDialogue(const TArray<FSideQuestDialogueLine>& Lines, AQuestInteractableActor* Source,
    ESideQuestStep ExpectedStep, ESideQuestStep ResultStep, bool bShouldAdvance)
{
    if (IsInDialogue()) return;

    DialogueSource = Source; ActiveDialogue = Lines; DialogueExpectedStep = ExpectedStep;
    DialogueResultStep = ResultStep; bDialogueAdvancesQuest = bShouldAdvance;
    DialogueIndex = ActiveDialogue.IsEmpty() ? INDEX_NONE : 0;
    NextDialogueAdvanceTime = GetWorld()->GetTimeSeconds() + 0.15f;
    if (IsInDialogue())
    {
        StopJumping();
        GetCharacterMovement()->StopMovementImmediately();
    }
    else
    {
        FinishDialogue();
    }
}

void ASideQuestCharacter::BeginEndingDialogue(const TArray<FSideQuestDialogueLine>& Lines)
{
    bDialogueEndsGame = true;
    BeginDialogue(Lines, nullptr, ESideQuestStep::Complete, ESideQuestStep::Complete, false);
}

void ASideQuestCharacter::FinishDialogue()
{
    if (bDialogueAdvancesQuest)
        if (ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>()) State->TryAdvanceQuest(DialogueExpectedStep, DialogueResultStep);
    if (DialogueSource) DialogueSource->CompleteInteraction();
    const bool bShouldEndGame = bDialogueEndsGame;
    bDialogueEndsGame = false;
    ActiveDialogue.Reset(); DialogueIndex = INDEX_NONE; DialogueSource = nullptr; CurrentInteractable = nullptr;
    if (bShouldEndGame)
    {
        if (ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>()) State->FinishFinalDialogue();
        return;
    }
    RefreshNearbyInteractable();
}

void ASideQuestCharacter::MoveForward(const FInputActionValue& Value)
{
    if (IsDead() || IsInDialogue() || IsGameplayLocked()) return;

    const FRotator ControlRotation = Controller ? Controller->GetControlRotation() : FRotator::ZeroRotator;
    AddMovementInput(FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::X), Value.Get<float>());
}

void ASideQuestCharacter::MoveRight(const FInputActionValue& Value)
{
    if (IsDead() || IsInDialogue() || IsGameplayLocked()) return;

    const FRotator ControlRotation = Controller ? Controller->GetControlRotation() : FRotator::ZeroRotator;
    AddMovementInput(FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::Y), Value.Get<float>());
}

void ASideQuestCharacter::LookYaw(const FInputActionValue& Value)
{
    AddControllerYawInput(Value.Get<float>());
}

void ASideQuestCharacter::LookPitch(const FInputActionValue& Value)
{
    AddControllerPitchInput(Value.Get<float>());
}

void ASideQuestCharacter::StartJump()
{
    if (!IsDead() && !IsInDialogue() && !IsGameplayLocked())
    {
        Jump();
    }
}

bool ASideQuestCharacter::IsDead() const
{
    return HealthComponent && HealthComponent->IsDead();
}

void ASideQuestCharacter::LightAttack()
{
    const float CurrentTime = GetWorld()->GetTimeSeconds();
    if (IsDead() || IsInDialogue() || IsGameplayLocked())
    {
        return;
    }

    if (!bSwordDrawn || bWeaponTransitioning)
    {
        PendingCombatAction = EPendingCombatAction::LightAttack;
        if (!bWeaponTransitioning) BeginDrawSword();
        return;
    }
    if (CurrentTime < NextAttackTime) return;

    if (CurrentTime - LastLightAttackTime > ComboResetTime)
        LightComboIndex = 0;

    UAnimMontage* Montage = LightAttackMontages.IsValidIndex(LightComboIndex)
        ? LightAttackMontages[LightComboIndex] : nullptr;
    PerformAttack(Montage, AttackDamage, AttackCooldown);
    LastLightAttackTime = CurrentTime;
    LightComboIndex = LightAttackMontages.IsEmpty() ? 0 : (LightComboIndex + 1) % LightAttackMontages.Num();
}

void ASideQuestCharacter::HeavyAttack()
{
    const float CurrentTime = GetWorld()->GetTimeSeconds();
    if (IsDead() || IsInDialogue() || IsGameplayLocked()) return;
    if (!bSwordDrawn || bWeaponTransitioning)
    {
        PendingCombatAction = EPendingCombatAction::HeavyAttack;
        if (!bWeaponTransitioning) BeginDrawSword();
        return;
    }
    if (CurrentTime < NextAttackTime) return;
    LightComboIndex = 0;
    PerformAttack(HeavyAttackMontage, HeavyAttackDamage, HeavyAttackCooldown);
}

void ASideQuestCharacter::PerformAttack(UAnimMontage* Montage, float Damage, float Cooldown)
{
    const float CurrentTime = GetWorld()->GetTimeSeconds();
    NextAttackTime = CurrentTime + Cooldown;
    LastCombatTime = CurrentTime;
    if (Montage) PlayAnimMontage(Montage);
    ScheduleAutoSheathe();

    const FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, 50.0f);
    const FVector End = Start + GetActorForwardVector() * AttackReach;
    FCollisionShape AttackShape = FCollisionShape::MakeSphere(AttackRadius);
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(PlayerSwordAttack), false, this);
    TArray<FHitResult> Hits;

    if (GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_Pawn, AttackShape, QueryParams))
    {
        TSet<AActor*> DamagedActors;
        for (const FHitResult& Hit : Hits)
        {
            AActor* HitActor = Hit.GetActor();
            if (HitActor && HitActor != this && !DamagedActors.Contains(HitActor) &&
                HitActor->FindComponentByClass<UHealthComponent>())
            {
                DamagedActors.Add(HitActor);
                UGameplayStatics::ApplyDamage(HitActor, Damage, GetController(), this, UDamageType::StaticClass());
                PlayAttackHitPresentation(HitActor, Hit.ImpactPoint);
            }
        }
    }
}

void ASideQuestCharacter::Dodge()
{
    const float CurrentTime = GetWorld()->GetTimeSeconds();
    if (IsDead() || IsInDialogue() || IsGameplayLocked() || bWeaponTransitioning || CurrentTime < NextAttackTime) return;

    DodgeEndTime = CurrentTime + DodgeDuration;
    NextAttackTime = DodgeEndTime;
    LastCombatTime = CurrentTime;
    if (DodgeMontage) PlayAnimMontage(DodgeMontage);

    FVector DodgeDirection = GetLastMovementInputVector().GetSafeNormal2D();
    if (DodgeDirection.IsNearlyZero()) DodgeDirection = GetActorForwardVector();
    LaunchCharacter(DodgeDirection * DodgeStrength, true, false);
    if (bSwordDrawn) ScheduleAutoSheathe();
}

void ASideQuestCharacter::BeginDrawSword()
{
    if (bSwordDrawn || bWeaponTransitioning || IsDead()) return;
    GetWorldTimerManager().ClearTimer(AutoSheatheTimer);
    bWeaponTransitioning = true;
    LastCombatTime = GetWorld()->GetTimeSeconds();
    if (DrawReachMontage) PlayAnimMontage(DrawReachMontage);
    GetWorldTimerManager().SetTimer(WeaponTransitionTimer, this, &ThisClass::FinishDrawReach,
        DrawReachDuration, false);
}

void ASideQuestCharacter::FinishDrawReach()
{
    AttachSwordToSocket(HandSwordSocket);
    if (DrawReadyMontage) PlayAnimMontage(DrawReadyMontage);
    GetWorldTimerManager().SetTimer(WeaponTransitionTimer, this, &ThisClass::FinishDrawSword,
        DrawReadyDuration, false);
}

void ASideQuestCharacter::FinishDrawSword()
{
    bSwordDrawn = true;
    bWeaponTransitioning = false;
    ExecutePendingCombatAction();
    ScheduleAutoSheathe();
}

void ASideQuestCharacter::BeginSheatheSword()
{
    if (!bSwordDrawn || bWeaponTransitioning || IsDead()) return;
    bWeaponTransitioning = true;
    if (SheatheSwordMontage) PlayAnimMontage(SheatheSwordMontage);
    GetWorldTimerManager().SetTimer(WeaponTransitionTimer, this, &ThisClass::FinishSheatheSwordPlacement,
        SheatheSwordDuration, false);
}

void ASideQuestCharacter::FinishSheatheSwordPlacement()
{
    AttachSwordToSocket(BackSwordSocket);
    bSwordDrawn = false;
    if (SheatheHandDownMontage) PlayAnimMontage(SheatheHandDownMontage);
    GetWorldTimerManager().SetTimer(WeaponTransitionTimer, this, &ThisClass::FinishSheatheSword,
        SheatheHandDownDuration, false);
}

void ASideQuestCharacter::FinishSheatheSword()
{
    bWeaponTransitioning = false;
    if (PendingCombatAction != EPendingCombatAction::None) BeginDrawSword();
}

void ASideQuestCharacter::ScheduleAutoSheathe()
{
    GetWorldTimerManager().ClearTimer(AutoSheatheTimer);
    if (bSwordDrawn && AutoSheatheDelay > 0.0f)
        GetWorldTimerManager().SetTimer(AutoSheatheTimer, this, &ThisClass::BeginSheatheSword,
            AutoSheatheDelay, false);
}

void ASideQuestCharacter::AttachSwordToSocket(FName SocketName)
{
    if (SwordMesh && GetMesh())
        SwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void ASideQuestCharacter::ExecutePendingCombatAction()
{
    const EPendingCombatAction Action = PendingCombatAction;
    PendingCombatAction = EPendingCombatAction::None;
    if (Action == EPendingCombatAction::LightAttack) LightAttack();
    else if (Action == EPendingCombatAction::HeavyAttack) HeavyAttack();
}

bool ASideQuestCharacter::IsDodging() const
{
    return GetWorld() && GetWorld()->GetTimeSeconds() < DodgeEndTime;
}

void ASideQuestCharacter::RestartAfterDeath()
{
    const ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>();
    if (IsDead() || (State && State->GetEndingState() == EEndingPresentationState::Credits))
    {
        if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
        {
            PlayerController->RestartLevel();
        }
    }
}

void ASideQuestCharacter::QuitFromCredits()
{
    const ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>();
    if (State && State->GetEndingState() == EEndingPresentationState::Credits)
    {
        UKismetSystemLibrary::QuitGame(this, Cast<APlayerController>(GetController()), EQuitPreference::Quit, false);
    }
}

bool ASideQuestCharacter::IsGameplayLocked() const
{
    const ASideQuestGameState* State = GetWorld() ? GetWorld()->GetGameState<ASideQuestGameState>() : nullptr;
    return State && State->HasEndingBegun();
}

void ASideQuestCharacter::HandleHealthChanged(UHealthComponent* Component, float NewHealth, float HealthDelta, AActor* DamageCauser)
{
    LastDamageTime = GetWorld()->GetTimeSeconds();
    LastCombatTime = LastDamageTime;
    if (bSwordDrawn) ScheduleAutoSheathe();
    if (HealthDelta < 0.0f && NewHealth > 0.0f && HitReactMontage)
    {
        PlayAnimMontage(HitReactMontage);
    }
    PlayHitPresentation();
    const ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>();
    if (!bMittensDamageBarkPlayed && HealthDelta < 0.0f && State &&
        State->GetQuestStep() == ESideQuestStep::FightMittens && Component->GetHealthFraction() <= 0.5f)
    {
        bMittensDamageBarkPlayed = true;
        if (APlayerController* PC = Cast<APlayerController>(GetController()))
            if (ASideQuestHUD* HUD = PC->GetHUD<ASideQuestHUD>())
                HUD->ShowGameplaySubtitle(FText::FromString(TEXT("Player")),
                    FText::FromString(TEXT("Mildred owes me substantially more than three gold!")), 3.0f);
    }
}

void ASideQuestCharacter::HandleDeath(UHealthComponent* Component, AActor* DamageCauser)
{
    StopJumping();
    GetWorldTimerManager().ClearTimer(WeaponTransitionTimer);
    GetWorldTimerManager().ClearTimer(AutoSheatheTimer);
    bWeaponTransitioning = false;
    PendingCombatAction = EPendingCombatAction::None;
    GetCharacterMovement()->DisableMovement();
    if (DeathMontage)
    {
        PlayAnimMontage(DeathMontage);
    }
    PlayDeathPresentation();
}
