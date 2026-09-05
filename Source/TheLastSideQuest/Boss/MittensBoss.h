#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/SideQuestInteractable.h"
#include "MittensBoss.generated.h"

class UBoxComponent;
class UHealthComponent;
class UPrimitiveComponent;
class UStaticMeshComponent;

/** The game's single, deliberately small final boss. C++ owns the encounter; Blueprint owns spectacle. */
UCLASS(Blueprintable)
class THELASTSIDEQUEST_API AMittensBoss : public ACharacter, public ISideQuestInteractable
{
    GENERATED_BODY()

public:
    AMittensBoss();
    virtual void Tick(float DeltaSeconds) override;
    virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;
    virtual FText GetInteractionLabel_Implementation() const override;
    virtual bool CanInteract_Implementation(APawn* Interactor) const override;
    virtual void Interact_Implementation(APawn* Interactor) override;

    UHealthComponent* GetHealthComponent() const { return HealthComponent; }
    bool IsEncounterActive() const { return bEncounterActive && !bDefeated; }
    bool IsDefeated() const { return bDefeated; }

    UFUNCTION(BlueprintImplementableEvent, Category="Mittens|Presentation") void PlayEncounterStartPresentation();
    UFUNCTION(BlueprintImplementableEvent, Category="Mittens|Presentation") void StartBossMusic();
    UFUNCTION(BlueprintImplementableEvent, Category="Mittens|Presentation") void StopBossMusic();
    UFUNCTION(BlueprintImplementableEvent, Category="Mittens|Presentation") void PlayPounceTelegraph();
    UFUNCTION(BlueprintImplementableEvent, Category="Mittens|Presentation") void PlayPounceImpact();
    UFUNCTION(BlueprintImplementableEvent, Category="Mittens|Presentation") void PlayScratchPresentation(int32 Beat);
    UFUNCTION(BlueprintImplementableEvent, Category="Mittens|Presentation") void PlayChaosDashPresentation();
    UFUNCTION(BlueprintImplementableEvent, Category="Mittens|Presentation") void PlayMeowPresentation();
    UFUNCTION(BlueprintImplementableEvent, Category="Mittens|Presentation") void PlayHitPresentation();
    UFUNCTION(BlueprintImplementableEvent, Category="Mittens|Presentation") void PlayDefeatPresentation();
    UFUNCTION(BlueprintImplementableEvent, Category="Mittens|Presentation") void PlayPickupPresentation(APawn* Carrier);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mittens|Combat") TObjectPtr<UHealthComponent> HealthComponent;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mittens|Arena") TObjectPtr<UBoxComponent> EntranceTrigger;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mittens|Arena") TObjectPtr<UBoxComponent> EntranceBlocker;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mittens|Arena") TObjectPtr<UStaticMeshComponent> EntranceVisual;
    /** Obvious source-only cat-sized proxy. Hide it in BP_MittensBoss when assigning the real skeletal mesh. */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mittens|Presentation") TObjectPtr<UStaticMeshComponent> PlaceholderCatBody;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mittens|Combat", meta=(ClampMin="0")) float PounceDamage = 18.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mittens|Combat", meta=(ClampMin="0")) float ScratchDamagePerBeat = 6.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mittens|Combat", meta=(ClampMin="0")) float ChaosDashDamage = 12.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mittens|Combat", meta=(ClampMin="0.1")) float AttackCooldown = 2.25f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mittens|Combat", meta=(ClampMin="50")) float ScratchRange = 175.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mittens|Combat", meta=(ClampMin="100")) float PounceRange = 900.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mittens|Combat", meta=(ClampMin="100")) float AttackHitRadius = 135.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mittens|Arena", meta=(ClampMin="300")) float ArenaRadius = 1100.0f;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mittens|Arena") FVector ArenaCenterOffset = FVector(-800, 0, 0);
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mittens|Arena") FVector EntranceTriggerOffset = FVector(-1600, 0, 80);
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Mittens|Arena") FVector EntranceBlockerOffset = FVector(-1900, 0, 220);

private:
    enum class EAttackState : uint8 { Idle, Telegraph, Moving, Scratch, Recovery };
    void BeginEncounter(APawn* Player);
    void ChooseAttack();
    void BeginPounce();
    void BeginScratch();
    void BeginChaosDash();
    void FinishAttack(float RecoverySeconds);
    void DamageTargetOnce(float Damage, float Radius);
    FVector ClampToArena(const FVector& Location) const;

    UFUNCTION() void HandleEntranceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION() void HandleHealthChanged(UHealthComponent* Component, float NewHealth, float HealthDelta, AActor* DamageCauser);
    UFUNCTION() void HandleDefeat(UHealthComponent* Component, AActor* DamageCauser);

    UPROPERTY() TObjectPtr<APawn> CombatTarget;
    FVector ArenaCenter;
    FVector MoveStart;
    FVector MoveTarget;
    float StateEndTime = 0.0f;
    float NextAttackTime = 0.0f;
    float MoveDuration = 0.0f;
    int32 ScratchBeat = 0;
    bool bEncounterActive = false;
    bool bDefeated = false;
    bool bDamageApplied = false;
    EAttackState AttackState = EAttackState::Idle;
};
