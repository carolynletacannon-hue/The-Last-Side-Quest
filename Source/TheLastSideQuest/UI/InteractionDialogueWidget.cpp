#include "UI/InteractionDialogueWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

void UInteractionDialogueWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>();
    WidgetTree->RootWidget = Root;

    ObjectiveText = WidgetTree->ConstructWidget<UTextBlock>();
    ObjectiveText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
    UCanvasPanelSlot* ObjectiveSlot = Root->AddChildToCanvas(ObjectiveText);
    ObjectiveSlot->SetAnchors(FAnchors(0.0f, 0.0f)); ObjectiveSlot->SetPosition(FVector2D(40, 35)); ObjectiveSlot->SetAutoSize(true);

    PromptText = WidgetTree->ConstructWidget<UTextBlock>();
    PromptText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.82f, 0.3f)));
    UCanvasPanelSlot* PromptSlot = Root->AddChildToCanvas(PromptText);
    PromptSlot->SetAnchors(FAnchors(0.5f, 0.72f)); PromptSlot->SetAlignment(FVector2D(0.5f)); PromptSlot->SetAutoSize(true);

    DialoguePanel = WidgetTree->ConstructWidget<UBorder>();
    DialoguePanel->SetBrushColor(FLinearColor(0.015f, 0.012f, 0.01f, 0.9f)); DialoguePanel->SetPadding(FMargin(24));
    UVerticalBox* DialogueBox = WidgetTree->ConstructWidget<UVerticalBox>();
    SpeakerText = WidgetTree->ConstructWidget<UTextBlock>(); DialogueText = WidgetTree->ConstructWidget<UTextBlock>();
    SpeakerText->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.72f, 0.2f)));
    DialogueText->SetAutoWrapText(true);
    DialogueBox->AddChildToVerticalBox(SpeakerText); DialogueBox->AddChildToVerticalBox(DialogueText); DialoguePanel->SetContent(DialogueBox);
    UCanvasPanelSlot* DialogueSlot = Root->AddChildToCanvas(DialoguePanel);
    DialogueSlot->SetAnchors(FAnchors(0.12f, 0.75f, 0.88f, 0.95f)); DialogueSlot->SetOffsets(FMargin(0));
    DialoguePanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UInteractionDialogueWidget::SetObjective(const FText& Text) { if (ObjectiveText) ObjectiveText->SetText(FText::Format(NSLOCTEXT("HUD", "Objective", "OBJECTIVE: {0}"), Text)); }
void UInteractionDialogueWidget::SetPrompt(const FText& Text, bool bVisible)
{
    if (!PromptText) return;
    PromptText->SetText(FText::Format(NSLOCTEXT("HUD", "Prompt", "[E] {0}"), Text));
    PromptText->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}
void UInteractionDialogueWidget::SetDialogue(const FText& Speaker, const FText& Text, bool bVisible)
{
    if (!DialoguePanel) return;
    SpeakerText->SetText(Speaker); DialogueText->SetText(Text);
    DialoguePanel->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}
