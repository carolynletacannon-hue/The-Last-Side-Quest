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
    virtual void BeginPlay() override;
    virtual void DrawHUD() override;
    void UpdateInteractionUI();

    /** Shows a short, non-modal line without locking movement, attacks, or interaction. */
    UFUNCTION(BlueprintCallable, Category = "UI|Subtitles")
    void ShowGameplaySubtitle(const FText& Speaker, const FText& Text, float Duration = 3.0f);

    bool HasGameplaySubtitle() const;

private:
    UPROPERTY() TObjectPtr<class UInteractionDialogueWidget> InteractionWidget;
    FText SubtitleSpeaker;
    FText SubtitleText;
    float SubtitleExpiresAt = 0.0f;
};
