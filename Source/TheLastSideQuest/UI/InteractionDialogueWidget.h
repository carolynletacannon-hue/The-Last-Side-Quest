#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Quest/SideQuestTypes.h"
#include "InteractionDialogueWidget.generated.h"

class UTextBlock;

/** Asset-free UMG view; a Blueprint subclass may replace its styling without owning dialogue logic. */
UCLASS(Blueprintable)
class THELASTSIDEQUEST_API UInteractionDialogueWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    void SetObjective(const FText& Text);
    void SetPrompt(const FText& Text, bool bVisible);
    void SetDialogue(const FText& Speaker, const FText& Text, bool bVisible);
    void SetGameplaySubtitle(const FText& Speaker, const FText& Text, bool bVisible);
    void SetEndingPresentation(EEndingPresentationState State, float FadeOpacity);
protected:
    virtual void NativeOnInitialized() override;

    /** Optional named hooks for a Widget Blueprint subclass. Omit anything your design does not need. */
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> ObjectiveText;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> PromptText;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> SpeakerText;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> DialogueText;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> SubtitleSpeakerText;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<UTextBlock> SubtitleBodyText;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<class UBorder> DialoguePanel;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<class UBorder> SubtitlePanel;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<class UBorder> QuestCompletePanel;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<class UBorder> FadePanel;
    UPROPERTY(BlueprintReadOnly, Category = "UI", meta = (BindWidgetOptional)) TObjectPtr<class UVerticalBox> CreditsPanel;

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Presentation")
    void OnDialogueVisibilityChanged(bool bVisible);

    UFUNCTION(BlueprintImplementableEvent, Category = "UI|Presentation")
    void OnPromptVisibilityChanged(bool bVisible);

private:
    void BuildFallbackLayout();
    bool bPreviousDialogueVisible = false;
    bool bPreviousPromptVisible = false;
};
