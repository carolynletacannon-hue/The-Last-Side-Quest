#include "UI/InteractionDialogueWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

namespace
{
UTextBlock* AddCenteredText(UWidgetTree* Tree, UVerticalBox* Box, const FText& Text, int32 Size, const FLinearColor& Color)
{
    UTextBlock* Label = Tree->ConstructWidget<UTextBlock>();
    Label->SetText(Text); Label->SetJustification(ETextJustify::Center); Label->SetColorAndOpacity(FSlateColor(Color));
    FSlateFontInfo Font = Label->GetFont(); Font.Size = Size; Label->SetFont(Font);
    UVerticalBoxSlot* Slot = Box->AddChildToVerticalBox(Label); Slot->SetHorizontalAlignment(HAlign_Center); Slot->SetPadding(FMargin(0, 7));
    return Label;
}
}

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

    QuestCompletePanel = WidgetTree->ConstructWidget<UBorder>();
    QuestCompletePanel->SetBrushColor(FLinearColor(0.02f, 0.015f, 0.008f, 0.94f)); QuestCompletePanel->SetPadding(FMargin(50, 32));
    UVerticalBox* CompleteBox = WidgetTree->ConstructWidget<UVerticalBox>();
    AddCenteredText(WidgetTree, CompleteBox, NSLOCTEXT("Ending", "Complete", "QUEST COMPLETE"), 42, FLinearColor(1.0f, .76f, .2f));
    AddCenteredText(WidgetTree, CompleteBox, NSLOCTEXT("Ending", "Title", "THE LAST SIDE QUEST"), 28, FLinearColor::White);
    AddCenteredText(WidgetTree, CompleteBox, NSLOCTEXT("Ending", "Reward", "Reward: 3 Gold"), 24, FLinearColor(1.0f, .82f, .35f));
    QuestCompletePanel->SetContent(CompleteBox);
    UCanvasPanelSlot* CompleteSlot = Root->AddChildToCanvas(QuestCompletePanel);
    CompleteSlot->SetAnchors(FAnchors(.5f,.42f)); CompleteSlot->SetAlignment(FVector2D(.5f)); CompleteSlot->SetAutoSize(true);
    QuestCompletePanel->SetVisibility(ESlateVisibility::Collapsed);

    FadePanel = WidgetTree->ConstructWidget<UBorder>(); FadePanel->SetBrushColor(FLinearColor::Black);
    UCanvasPanelSlot* FadeSlot = Root->AddChildToCanvas(FadePanel); FadeSlot->SetAnchors(FAnchors(0,0,1,1)); FadeSlot->SetOffsets(FMargin(0));
    FadePanel->SetVisibility(ESlateVisibility::Collapsed);

    CreditsPanel = WidgetTree->ConstructWidget<UVerticalBox>();
    AddCenteredText(WidgetTree, CreditsPanel, NSLOCTEXT("Ending", "CreditsTitle", "THE LAST SIDE QUEST"), 46, FLinearColor(1.0f, .76f, .2f));
    AddCenteredText(WidgetTree, CreditsPanel, NSLOCTEXT("Ending", "CreatedBy", "Created by Carolyn Cannon"), 25, FLinearColor::White);
    AddCenteredText(WidgetTree, CreditsPanel, NSLOCTEXT("Ending", "Engine", "Made in Unreal Engine"), 21, FLinearColor(.8f,.8f,.8f));
    AddCenteredText(WidgetTree, CreditsPanel, NSLOCTEXT("Ending", "Attribution", "Asset and audio attribution: see included project documentation"), 17, FLinearColor(.58f,.58f,.58f));
    AddCenteredText(WidgetTree, CreditsPanel, NSLOCTEXT("Ending", "Thanks", "Thanks for playing."), 25, FLinearColor::White);
    AddCenteredText(WidgetTree, CreditsPanel, NSLOCTEXT("Ending", "CreditsControls", "R / Menu: Restart     Escape / B: Quit"), 17, FLinearColor(.72f,.72f,.72f));
    UCanvasPanelSlot* CreditsSlot = Root->AddChildToCanvas(CreditsPanel);
    CreditsSlot->SetAnchors(FAnchors(.5f,.5f)); CreditsSlot->SetAlignment(FVector2D(.5f)); CreditsSlot->SetAutoSize(true);
    CreditsPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UInteractionDialogueWidget::SetEndingPresentation(EEndingPresentationState State, float FadeOpacity)
{
    const bool bEnding = State != EEndingPresentationState::None;
    if (ObjectiveText) ObjectiveText->SetVisibility(bEnding ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
    if (QuestCompletePanel) QuestCompletePanel->SetVisibility(State == EEndingPresentationState::QuestComplete ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
    if (FadePanel)
    {
        FadePanel->SetBrushColor(FLinearColor(0, 0, 0, FadeOpacity));
        FadePanel->SetVisibility((State == EEndingPresentationState::Fading || State == EEndingPresentationState::Credits) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
    }
    if (CreditsPanel) CreditsPanel->SetVisibility(State == EEndingPresentationState::Credits ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
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
