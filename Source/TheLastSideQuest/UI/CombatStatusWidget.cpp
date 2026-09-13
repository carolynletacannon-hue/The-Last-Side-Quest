#include "UI/CombatStatusWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UCombatStatusWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (!WidgetTree->RootWidget)
    {
        BuildFallbackLayout();
    }
}

void UCombatStatusWidget::BuildFallbackLayout()
{
    UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>();
    WidgetTree->RootWidget = Root;

    UVerticalBox* PlayerPanel = WidgetTree->ConstructWidget<UVerticalBox>();
    PlayerHealthText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("PlayerHealthText"));
    PlayerHealthBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("PlayerHealthBar"));
    PlayerHealthBar->SetFillColorAndOpacity(FLinearColor(.72f, .035f, .025f));
    PlayerPanel->AddChildToVerticalBox(PlayerHealthText);
    PlayerPanel->AddChildToVerticalBox(PlayerHealthBar);
    UCanvasPanelSlot* PlayerSlot = Root->AddChildToCanvas(PlayerPanel);
    PlayerSlot->SetAnchors(FAnchors(0.f, 1.f));
    PlayerSlot->SetAlignment(FVector2D(0.f, 1.f));
    PlayerSlot->SetPosition(FVector2D(40.f, -40.f));
    PlayerSlot->SetSize(FVector2D(300.f, 48.f));

    BossPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("BossPanel"));
    BossPanel->SetBrushColor(FLinearColor(.01f, .005f, .015f, .92f));
    BossPanel->SetPadding(FMargin(10.f));
    UVerticalBox* BossBox = WidgetTree->ConstructWidget<UVerticalBox>();
    BossNameText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("BossNameText"));
    BossNameText->SetJustification(ETextJustify::Center);
    BossNameText->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, .78f, .3f)));
    BossHealthBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("BossHealthBar"));
    BossHealthBar->SetFillColorAndOpacity(FLinearColor(.48f, .03f, .65f));
    BossHealthText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("BossHealthText"));
    BossHealthText->SetJustification(ETextJustify::Center);
    BossBox->AddChildToVerticalBox(BossNameText);
    BossBox->AddChildToVerticalBox(BossHealthBar);
    BossBox->AddChildToVerticalBox(BossHealthText);
    BossPanel->SetContent(BossBox);
    UCanvasPanelSlot* BossSlot = Root->AddChildToCanvas(BossPanel);
    BossSlot->SetAnchors(FAnchors(.2f, .84f, .8f, .84f));
    BossSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 78.f));
    BossPanel->SetVisibility(ESlateVisibility::Collapsed);

    DamageFlash = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("DamageFlash"));
    DamageFlash->SetBrushColor(FLinearColor(.65f, 0.f, 0.f, 1.f));
    DamageFlash->SetRenderOpacity(0.f);
    UCanvasPanelSlot* DamageSlot = Root->AddChildToCanvas(DamageFlash);
    DamageSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
    DamageSlot->SetOffsets(FMargin(0.f));

    DefeatedPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("DefeatedPanel"));
    DefeatedPanel->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, .75f));
    UTextBlock* DefeatedText = WidgetTree->ConstructWidget<UTextBlock>();
    DefeatedText->SetText(NSLOCTEXT("HUD", "Defeated", "YOU HAVE FALLEN\n\nPress R / Menu to restart"));
    DefeatedText->SetJustification(ETextJustify::Center);
    DefeatedPanel->SetContent(DefeatedText);
    UCanvasPanelSlot* DefeatedSlot = Root->AddChildToCanvas(DefeatedPanel);
    DefeatedSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
    DefeatedSlot->SetOffsets(FMargin(0.f));
    DefeatedPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UCombatStatusWidget::SetPlayerHealth(float Health, float MaxHealth)
{
    const float Fraction = MaxHealth > 0.f ? FMath::Clamp(Health / MaxHealth, 0.f, 1.f) : 0.f;
    if (PlayerHealthBar) PlayerHealthBar->SetPercent(Fraction);
    if (PlayerHealthText) PlayerHealthText->SetText(FText::Format(NSLOCTEXT("HUD", "HealthValue", "{0} / {1}"), FMath::RoundToInt(Health), FMath::RoundToInt(MaxHealth)));
    if (!FMath::IsNearlyEqual(Health, PreviousPlayerHealth) || !FMath::IsNearlyEqual(MaxHealth, PreviousPlayerMaxHealth))
    {
        OnPlayerHealthChanged(Fraction, Health, MaxHealth);
        PreviousPlayerHealth = Health;
        PreviousPlayerMaxHealth = MaxHealth;
    }
}

void UCombatStatusWidget::SetBossHealth(const FText& Name, float Health, float MaxHealth, bool bVisible)
{
    const float Fraction = MaxHealth > 0.f ? FMath::Clamp(Health / MaxHealth, 0.f, 1.f) : 0.f;
    if (BossPanel) BossPanel->SetVisibility(bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
    if (BossNameText) BossNameText->SetText(Name);
    if (BossHealthBar) BossHealthBar->SetPercent(Fraction);
    if (BossHealthText) BossHealthText->SetText(FText::Format(NSLOCTEXT("HUD", "BossHealthValue", "{0} / {1}"), FMath::RoundToInt(Health), FMath::RoundToInt(MaxHealth)));
    if (!FMath::IsNearlyEqual(Health, PreviousBossHealth) || !FMath::IsNearlyEqual(MaxHealth, PreviousBossMaxHealth) || bVisible != bPreviousBossVisible)
    {
        OnBossHealthChanged(Fraction, Health, MaxHealth, bVisible);
        PreviousBossHealth = Health;
        PreviousBossMaxHealth = MaxHealth;
        bPreviousBossVisible = bVisible;
    }
}

void UCombatStatusWidget::SetDamageFlash(float Opacity)
{
    if (DamageFlash) DamageFlash->SetRenderOpacity(FMath::Clamp(Opacity, 0.f, 1.f));
    OnDamageFlashChanged(Opacity);
}

void UCombatStatusWidget::SetDefeated(bool bDefeated)
{
    if (DefeatedPanel) DefeatedPanel->SetVisibility(bDefeated ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
    if (bDefeated != bPreviousDefeated)
    {
        OnDefeatedChanged(bDefeated);
        bPreviousDefeated = bDefeated;
    }
}
