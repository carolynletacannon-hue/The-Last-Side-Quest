#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SideQuestHUD.generated.h"

/** Asset-free combat HUD so health, damage, and restart state remain readable in a source-only map. */
UCLASS()
class THELASTSIDEQUEST_API ASideQuestHUD : public AHUD
{
    GENERATED_BODY()

public:
    ASideQuestHUD();
    virtual void BeginPlay() override;
    virtual void DrawHUD() override;
    void UpdateInteractionUI();

    /** Shows a short, non-modal line without locking movement, attacks, or interaction. */
    UFUNCTION(BlueprintCallable, Category = "UI|Subtitles")
    void ShowGameplaySubtitle(const FText& Speaker, const FText& Text, float Duration = 3.0f);

    bool HasGameplaySubtitle() const;

protected:
    /** Replace these with Widget Blueprint subclasses to style the entire HUD in UMG. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UInteractionDialogueWidget> InteractionWidgetClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UCombatStatusWidget> CombatWidgetClass;

private:
    UPROPERTY() TObjectPtr<class UInteractionDialogueWidget> InteractionWidget;
    UPROPERTY() TObjectPtr<class UCombatStatusWidget> CombatWidget;
    FText SubtitleSpeaker;
    FText SubtitleText;
    float SubtitleExpiresAt = 0.0f;
};
