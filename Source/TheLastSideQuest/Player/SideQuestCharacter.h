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
class UStaticMeshComponent;
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
    virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;

    UHealthComponent* GetHealthComponent() const { return HealthComponent; }
    bool IsDead() const;
    float GetLastDamageTime() const { return LastDamageTime; }

    UFUNCTION(BlueprintPure, Category = "Combat")
    float GetLastCombatTime() const { return LastCombatTime; }

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsDodging() const;

    UFUNCTION(BlueprintPure, Category = "Combat|Weapon")
    bool IsSwordDrawn() const { return bSwordDrawn; }

    UFUNCTION(BlueprintPure, Category = "Combat|Weapon")
    bool IsWeaponTransitioning() const { return bWeaponTransitioning; }
    bool HasAvailableInteraction() const { return IsValid(CurrentInteractable); }
    bool IsInDialogue() const { return DialogueIndex != INDEX_NONE; }
    FText GetCurrentInteractionLabel() const;
    FText GetDialogueSpeaker() const;
    FText GetDialogueText() const;
    void BeginDialogue(const TArray<FSideQuestDialogueLine>& Lines, AQuestInteractableActor* Source,
        ESideQuestStep ExpectedStep, ESideQuestStep ResultStep, bool bShouldAdvance);
    void BeginEndingDialogue(const TArray<FSideQuestDialogueLine>& Lines);
    bool IsGameplayLocked() const;

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
    void LightAttack();
    void HeavyAttack();
    void Dodge();
    void PerformAttack(UAnimMontage* Montage, float Damage, float Cooldown);
    void BeginDrawSword();
    void FinishDrawReach();
    void FinishDrawSword();
    void BeginSheatheSword();
    void FinishSheatheSwordPlacement();
    void FinishSheatheSword();
    void ScheduleAutoSheathe();
    void AttachSwordToSocket(FName SocketName);
    void ExecutePendingCombatAction();
    void RestartAfterDeath();
    void QuitFromCredits();
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

    /** Assign the imported sword mesh in BP_SideQuestCharacter. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UStaticMeshComponent> SwordMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation", meta = (AllowPrivateAccess = "true"))
    TArray<TObjectPtr<UAnimMontage>> LightAttackMontages;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> HeavyAttackMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> DodgeMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> HitReactMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Animation", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> DeathMontage;

    /** Reach behind the shoulder and close the hand around the still-sheathed sword. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> DrawReachMontage;

    /** Pull the sword free and bring it down into the two-handed ready pose. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> DrawReadyMontage;

    /** Move the held sword into the back-mounted sheath. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> SheatheSwordMontage;

    /** Lower the now-empty hand after the sword has been placed in the sheath. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAnimMontage> SheatheHandDownMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
    FName BackSwordSocket = TEXT("Sword_Back");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true"))
    FName HandSwordSocket = TEXT("Sword_Hand");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true", ClampMin = "0.01"))
    float DrawReachDuration = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true", ClampMin = "0.01"))
    float DrawReadyDuration = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true", ClampMin = "0.01"))
    float SheatheSwordDuration = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true", ClampMin = "0.01"))
    float SheatheHandDownDuration = 0.4f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Weapon", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float AutoSheatheDelay = 4.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float AttackDamage = 34.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float HeavyAttackDamage = 55.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "25.0"))
    float AttackReach = 165.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "1.0"))
    float AttackRadius = 55.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.1"))
    float AttackCooldown = 0.55f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.1"))
    float HeavyAttackCooldown = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float ComboResetTime = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float DodgeStrength = 650.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float DodgeDuration = 0.55f;

    float NextAttackTime = 0.0f;
    float LastDamageTime = -100.0f;
    float LastCombatTime = -100.0f;
    float LastLightAttackTime = -100.0f;
    float DodgeEndTime = -100.0f;
    int32 LightComboIndex = 0;
    enum class EPendingCombatAction : uint8 { None, LightAttack, HeavyAttack };
    EPendingCombatAction PendingCombatAction = EPendingCombatAction::None;
    bool bSwordDrawn = false;
    bool bWeaponTransitioning = false;
    bool bMittensDamageBarkPlayed = false;
    FTimerHandle WeaponTransitionTimer;
    FTimerHandle AutoSheatheTimer;

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
    TObjectPtr<UInputAction> HeavyAttackAction;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    TObjectPtr<UInputAction> DodgeAction;

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
    bool bDialogueEndsGame = false;
    float NextDialogueAdvanceTime = 0.0f;
    FTimerHandle InteractionScanTimer;
    UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> InteractAction;
    UPROPERTY(EditDefaultsOnly, Category = "Input") TObjectPtr<UInputAction> QuitAction;
};
