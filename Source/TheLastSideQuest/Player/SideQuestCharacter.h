#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Quest/SideQuestTypes.h"
#include "SideQuestCharacter.generated.h"

class UCameraComponent;
class UHealthComponent;
class UInputAction;
class UInputMappingContext;
class UAnimMontage;
class USpringArmComponent;
class AQuestInteractableActor;
struct FInputActionValue;

/** Third-person player foundation for The Last Side Quest. */
UCLASS(Blueprintable)
class THELASTSIDEQUEST_API ASideQuestCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASideQuestCharacter();

    UHealthComponent* GetHealthComponent() const { return HealthComponent; }
    bool IsDead() const;
    float GetLastDamageTime() const { return LastDamageTime; }
    bool HasAvailableInteraction() const { return IsValid(CurrentInteractable); }
    bool IsInDialogue() const { return DialogueIndex != INDEX_NONE; }
    FText GetCurrentInteractionLabel() const;
    FText GetDialogueSpeaker() const;
    FText GetDialogueText() const;
    void BeginDialogue(const TArray<FSideQuestDialogueLine>& Lines, AQuestInteractableActor* Source,
        ESideQuestStep ExpectedStep, ESideQuestStep ResultStep, bool bShouldAdvance);

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat|Presentation")
    void PlayHitPresentation();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat|Presentation")
    void PlayDeathPresentation();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat|Presentation")
    void PlayAttackHitPresentation(AActor* HitActor, FVector HitLocation);

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
    void MoveForward(const FInputActionValue& Value);
    void MoveRight(const FInputActionValue& Value);
    void LookYaw(const FInputActionValue& Value);
    void LookPitch(const FInputActionValue& Value);
    void StartJump();
    void Attack();
    void RestartAfterDeath();
    void Interact();
    void RefreshNearbyInteractable();
    void FinishDialogue();

    UFUNCTION()
    void HandleHealthChanged(UHealthComponent* Component, float NewHealth, float HealthDelta, AActor* DamageCauser);

    UFUNCTION()
    void HandleDeath(UHealthComponent* Component, AActor* DamageCauser);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UHealthComponent> HealthComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> AttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float AttackDamage = 34.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "25.0"))
    float AttackReach = 165.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "1.0"))
    float AttackRadius = 55.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.1"))
    float AttackCooldown = 0.55f;

    float NextAttackTime = 0.0f;
    float LastDamageTime = -100.0f;

    /** Replace these defaults in a presentation Blueprint if different bindings are required. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveForwardAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> MoveRightAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> LookYawAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> LookPitchAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> AttackAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> RestartAction;

    UPROPERTY(EditAnywhere, Category = "Interaction", meta=(ClampMin="50.0")) float InteractionRadius = 225.0f;
    UPROPERTY() TObjectPtr<AActor> CurrentInteractable;
    UPROPERTY() TObjectPtr<AQuestInteractableActor> DialogueSource;
    UPROPERTY() TArray<FSideQuestDialogueLine> ActiveDialogue;
    int32 DialogueIndex = INDEX_NONE;
    ESideQuestStep DialogueExpectedStep = ESideQuestStep::TalkToMildred;
    ESideQuestStep DialogueResultStep = ESideQuestStep::TalkToGuard;
    bool bDialogueAdvancesQuest = false;
    FTimerHandle InteractionScanTimer;
    UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> InteractAction;
};
