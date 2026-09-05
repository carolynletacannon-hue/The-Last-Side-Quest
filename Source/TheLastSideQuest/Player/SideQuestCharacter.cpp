#include "Player/SideQuestCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputCoreTypes.h"

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

    DefaultMappingContext = CreateDefaultSubobject<UInputMappingContext>(TEXT("DefaultMappingContext"));
    MoveForwardAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveForwardAction"));
    MoveRightAction = CreateDefaultSubobject<UInputAction>(TEXT("MoveRightAction"));
    LookYawAction = CreateDefaultSubobject<UInputAction>(TEXT("LookYawAction"));
    LookPitchAction = CreateDefaultSubobject<UInputAction>(TEXT("LookPitchAction"));
    JumpAction = CreateDefaultSubobject<UInputAction>(TEXT("JumpAction"));

    MoveForwardAction->ValueType = EInputActionValueType::Axis1D;
    MoveRightAction->ValueType = EInputActionValueType::Axis1D;
    LookYawAction->ValueType = EInputActionValueType::Axis1D;
    LookPitchAction->ValueType = EInputActionValueType::Axis1D;
    JumpAction->ValueType = EInputActionValueType::Boolean;

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
}

void ASideQuestCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ASideQuestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
    EnhancedInput->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ThisClass::MoveForward);
    EnhancedInput->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ThisClass::MoveRight);
    EnhancedInput->BindAction(LookYawAction, ETriggerEvent::Triggered, this, &ThisClass::LookYaw);
    EnhancedInput->BindAction(LookPitchAction, ETriggerEvent::Triggered, this, &ThisClass::LookPitch);
    EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
    EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
}

void ASideQuestCharacter::MoveForward(const FInputActionValue& Value)
{
    const FRotator ControlRotation = Controller ? Controller->GetControlRotation() : FRotator::ZeroRotator;
    AddMovementInput(FRotationMatrix(FRotator(0.0f, ControlRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::X), Value.Get<float>());
}

void ASideQuestCharacter::MoveRight(const FInputActionValue& Value)
{
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
