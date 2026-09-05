#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
protected:
    virtual void NativeOnInitialized() override;
private:
    UPROPERTY() TObjectPtr<UTextBlock> ObjectiveText;
    UPROPERTY() TObjectPtr<UTextBlock> PromptText;
    UPROPERTY() TObjectPtr<UTextBlock> SpeakerText;
    UPROPERTY() TObjectPtr<UTextBlock> DialogueText;
    UPROPERTY() TObjectPtr<class UBorder> DialoguePanel;
};
