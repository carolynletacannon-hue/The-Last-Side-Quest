#include "UI/SideQuestHUD.h"

#include "Combat/HealthComponent.h"
#include "Boss/MittensBoss.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Player/SideQuestCharacter.h"
#include "Quest/SideQuestGameState.h"
#include "UI/InteractionDialogueWidget.h"
#include "UI/CombatStatusWidget.h"

ASideQuestHUD::ASideQuestHUD()
{
    InteractionWidgetClass = UInteractionDialogueWidget::StaticClass();
    CombatWidgetClass = UCombatStatusWidget::StaticClass();
}

void ASideQuestHUD::BeginPlay()
{
    Super::BeginPlay();
    if (APlayerController* PC = GetOwningPlayerController())
    {
        InteractionWidget = CreateWidget<UInteractionDialogueWidget>(PC, InteractionWidgetClass);
        if (InteractionWidget) InteractionWidget->AddToViewport(10);
        CombatWidget = CreateWidget<UCombatStatusWidget>(PC, CombatWidgetClass);
        if (CombatWidget) CombatWidget->AddToViewport(0);
    }
}

void ASideQuestHUD::UpdateInteractionUI()
{
    ASideQuestCharacter* Player = Cast<ASideQuestCharacter>(GetOwningPawn());
    ASideQuestGameState* State = GetWorld() ? GetWorld()->GetGameState<ASideQuestGameState>() : nullptr;
    if (!InteractionWidget || !Player || !State) return;
    InteractionWidget->SetObjective(State->GetObjectiveText());
    InteractionWidget->SetPrompt(Player->GetCurrentInteractionLabel(), Player->HasAvailableInteraction() && !Player->IsInDialogue());
    InteractionWidget->SetDialogue(Player->GetDialogueSpeaker(), Player->GetDialogueText(), Player->IsInDialogue());
    InteractionWidget->SetGameplaySubtitle(SubtitleSpeaker, SubtitleText,
        HasGameplaySubtitle() && !Player->IsInDialogue());
    InteractionWidget->SetEndingPresentation(State->GetEndingState(), State->GetFadeOpacity());
}

void ASideQuestHUD::ShowGameplaySubtitle(const FText& Speaker, const FText& Text, float Duration)
{
    SubtitleSpeaker = Speaker;
    SubtitleText = Text;
    SubtitleExpiresAt = GetWorld() ? GetWorld()->GetTimeSeconds() + FMath::Max(Duration, 0.5f) : 0.0f;
}

bool ASideQuestHUD::HasGameplaySubtitle() const
{
    return GetWorld() && !SubtitleText.IsEmpty() && GetWorld()->GetTimeSeconds() < SubtitleExpiresAt;
}

void ASideQuestHUD::DrawHUD()
{
    Super::DrawHUD();
    UpdateInteractionUI();

    const ASideQuestCharacter* Player = GetOwningPawn() ? Cast<ASideQuestCharacter>(GetOwningPawn()) : nullptr;
    if (!Player || !Canvas)
    {
        return;
    }

    const ASideQuestGameState* State = GetWorld()->GetGameState<ASideQuestGameState>();
    if (State && State->HasEndingBegun())
    {
        if (CombatWidget) CombatWidget->SetVisibility(ESlateVisibility::Collapsed);
        return;
    }
    if (CombatWidget) CombatWidget->SetVisibility(ESlateVisibility::HitTestInvisible);

    const UHealthComponent* Health = Player->GetHealthComponent();
    if (CombatWidget) CombatWidget->SetPlayerHealth(Health->GetHealth(), Health->GetMaxHealth());

    const float DamageAge = GetWorld()->GetTimeSeconds() - Player->GetLastDamageTime();
    if (DamageAge < 0.25f)
    {
        if (CombatWidget) CombatWidget->SetDamageFlash(0.2f * (1.0f - DamageAge / 0.25f));
    }
    else if (CombatWidget) CombatWidget->SetDamageFlash(0.f);

    bool bBossVisible = false;
    for (TActorIterator<AMittensBoss> It(GetWorld()); It; ++It)
    {
        if (!It->IsEncounterActive()) continue;
        const UHealthComponent* BossHealth = It->GetHealthComponent();
        if (CombatWidget) CombatWidget->SetBossHealth(NSLOCTEXT("HUD", "MittensBossName", "MITTENS — DEVOURER OF WORLDS"), BossHealth->GetHealth(), BossHealth->GetMaxHealth(), true);
        bBossVisible = true;
        break;
    }
    if (!bBossVisible && CombatWidget) CombatWidget->SetBossHealth(FText::GetEmpty(), 0.f, 1.f, false);

    if (CombatWidget) CombatWidget->SetDefeated(Player->IsDead());
}
