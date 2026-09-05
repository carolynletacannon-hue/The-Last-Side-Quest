#include "UI/SideQuestHUD.h"

#include "Combat/HealthComponent.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Player/SideQuestCharacter.h"
#include "Quest/SideQuestGameState.h"
#include "UI/InteractionDialogueWidget.h"

void ASideQuestHUD::BeginPlay()
{
    Super::BeginPlay();
    if (APlayerController* PC = GetOwningPlayerController())
    {
        InteractionWidget = CreateWidget<UInteractionDialogueWidget>(PC, UInteractionDialogueWidget::StaticClass());
        if (InteractionWidget) InteractionWidget->AddToViewport();
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

    const UHealthComponent* Health = Player->GetHealthComponent();
    const float BarX = 40.0f;
    const float BarY = Canvas->ClipY - 70.0f;
    const float BarWidth = 280.0f;
    const float BarHeight = 22.0f;
    DrawRect(FLinearColor(0.02f, 0.02f, 0.02f, 0.85f), BarX - 3.0f, BarY - 3.0f, BarWidth + 6.0f, BarHeight + 6.0f);
    DrawRect(FLinearColor(0.7f, 0.05f, 0.03f), BarX, BarY, BarWidth * Health->GetHealthFraction(), BarHeight);
    DrawText(FString::Printf(TEXT("HEALTH  %.0f / %.0f"), Health->GetHealth(), Health->GetMaxHealth()),
        FLinearColor::White, BarX, BarY - 25.0f, GEngine->GetSmallFont(), 1.0f, false);

    const float DamageAge = GetWorld()->GetTimeSeconds() - Player->GetLastDamageTime();
    if (DamageAge < 0.25f)
    {
        DrawRect(FLinearColor(0.65f, 0.0f, 0.0f, 0.2f * (1.0f - DamageAge / 0.25f)), 0.0f, 0.0f, Canvas->ClipX, Canvas->ClipY);
    }

    if (Player->IsDead())
    {
        DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 0.72f), 0.0f, 0.0f, Canvas->ClipX, Canvas->ClipY);
        const FString Defeated = TEXT("YOU HAVE FALLEN");
        const FString Restart = TEXT("Press R / Menu to restart");
        float TextWidth = 0.0f;
        float TextHeight = 0.0f;
        GetTextSize(Defeated, TextWidth, TextHeight, GEngine->GetLargeFont(), 1.5f);
        DrawText(Defeated, FLinearColor(0.9f, 0.12f, 0.06f), (Canvas->ClipX - TextWidth) * 0.5f,
            Canvas->ClipY * 0.42f, GEngine->GetLargeFont(), 1.5f, false);
        GetTextSize(Restart, TextWidth, TextHeight, GEngine->GetMediumFont());
        DrawText(Restart, FLinearColor::White, (Canvas->ClipX - TextWidth) * 0.5f,
            Canvas->ClipY * 0.53f, GEngine->GetMediumFont(), 1.0f, false);
    }
}
