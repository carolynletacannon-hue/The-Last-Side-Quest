#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class AActor;
class AController;
class UDamageType;
class UHealthComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FHealthChangedSignature, UHealthComponent*, HealthComponent,
    float, NewHealth, float, HealthDelta, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDeathSignature, UHealthComponent*, HealthComponent, AActor*, DamageCauser);

/** Small reusable health component shared by the player and every normal enemy. */
UCLASS(ClassGroup = (Combat), BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class THELASTSIDEQUEST_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHealthComponent();

    UFUNCTION(BlueprintPure, Category = "Combat|Health")
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintPure, Category = "Combat|Health")
    float GetMaxHealth() const { return MaxHealth; }

    UFUNCTION(BlueprintPure, Category = "Combat|Health")
    float GetHealthFraction() const { return MaxHealth > 0.0f ? Health / MaxHealth : 0.0f; }

    UFUNCTION(BlueprintPure, Category = "Combat|Health")
    bool IsDead() const { return Health <= 0.0f; }

    /** Intended for constructors/setup before BeginPlay initializes current health. */
    void SetMaxHealth(float NewMaxHealth) { MaxHealth = FMath::Max(1.0f, NewMaxHealth); }

    UPROPERTY(BlueprintAssignable, Category = "Combat|Health")
    FHealthChangedSignature OnHealthChanged;

    UPROPERTY(BlueprintAssignable, Category = "Combat|Health")
    FDeathSignature OnDeath;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Health", meta = (ClampMin = "1.0"))
    float MaxHealth = 100.0f;

private:
    UFUNCTION()
    void HandleOwnerDamaged(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
        AController* InstigatedBy, AActor* DamageCauser);

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Combat|Health", meta = (AllowPrivateAccess = "true"))
    float Health = 100.0f;
};
