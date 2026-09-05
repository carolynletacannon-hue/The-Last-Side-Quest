#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SideQuestEnemy.generated.h"

class UPawnSensingComponent;
class UHealthComponent;

/** One tunable normal-enemy class; Blueprint children provide goblin, slime, or skeleton presentation and stats. */
UCLASS(Blueprintable)
class THELASTSIDEQUEST_API ASideQuestEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    ASideQuestEnemy();

    float GetAttackDamage() const { return AttackDamage; }
    float GetAttackRange() const { return AttackRange; }
    float GetAttackCooldown() const { return AttackCooldown; }
    float GetTargetMemoryDuration() const { return TargetMemoryDuration; }
    UHealthComponent* GetHealthComponent() const { return HealthComponent; }

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat|Presentation")
    void PlayAttackPresentation();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat|Presentation")
    void PlayHitPresentation();

    UFUNCTION(BlueprintImplementableEvent, Category = "Combat|Presentation")
    void PlayDeathPresentation();

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void HandlePawnSeen(APawn* SeenPawn);

    UFUNCTION()
    void HandleHealthChanged(UHealthComponent* Component, float NewHealth, float HealthDelta, AActor* DamageCauser);

    UFUNCTION()
    void HandleDeath(UHealthComponent* Component, AActor* DamageCauser);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UHealthComponent> HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UPawnSensingComponent> PawnSensing;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float AttackDamage = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "50.0"))
    float AttackRange = 140.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.1"))
    float AttackCooldown = 1.25f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float TargetMemoryDuration = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
    float DestroyDelay = 4.0f;
};
