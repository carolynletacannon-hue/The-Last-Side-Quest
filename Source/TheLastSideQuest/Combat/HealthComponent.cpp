#include "Combat/HealthComponent.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;

    if (AActor* Owner = GetOwner())
    {
        Owner->OnTakeAnyDamage.AddDynamic(this, &ThisClass::HandleOwnerDamaged);
    }
}

void UHealthComponent::HandleOwnerDamaged(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
    AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead())
    {
        return;
    }

    const float PreviousHealth = Health;
    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(this, Health, Health - PreviousHealth, DamageCauser);

    if (IsDead())
    {
        OnDeath.Broadcast(this, DamageCauser);
    }
}
