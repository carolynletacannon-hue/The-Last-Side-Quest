#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatStatusWidget.generated.h"

class UBorder;
class UProgressBar;
class UTextBlock;

/**
 * Combat presentation owned by UMG. Create a Widget Blueprint subclass and use
 * the optional widget names below; C++ continues to own all gameplay values.
 */
UCLASS(Blueprintable)
class THELASTSIDEQUEST_API UCombatStatusWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetPlayerHealth(float Health, float MaxHealth);
    void SetBossHealth(const FText& Name, float Health, float MaxHealth, bool bVisible);
    void SetDamageFlash(float Opacity);
    void SetDefeated(bool bDefeated);

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UProgressBar> PlayerHealthBar;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> PlayerHealthText;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UBorder> BossPanel;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UProgressBar> BossHealthBar;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> BossNameText;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> BossHealthText;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UBorder> DamageFlash;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UBorder> DefeatedPanel;

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Presentation")
    void OnPlayerHealthChanged(float HealthFraction, float Health, float MaxHealth);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Presentation")
    void OnBossHealthChanged(float HealthFraction, float Health, float MaxHealth, bool bVisible);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Presentation")
    void OnDamageFlashChanged(float Opacity);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Presentation")
    void OnDefeatedChanged(bool bDefeated);

private:
    void BuildFallbackLayout();
    float PreviousPlayerHealth = -1.f;
    float PreviousPlayerMaxHealth = -1.f;
    float PreviousBossHealth = -1.f;
    float PreviousBossMaxHealth = -1.f;
    bool bPreviousBossVisible = false;
    bool bPreviousDefeated = false;
};
