#pragma once
#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Quest/SideQuestTypes.h"
#include "QuestProgressTrigger.generated.h"

/** One-shot route marker for forest clue, ruins entrance, and boss-room arrival. */
UCLASS(Blueprintable)
class THELASTSIDEQUEST_API AQuestProgressTrigger : public ATriggerBox
{
    GENERATED_BODY()
public:
    AQuestProgressTrigger();
    void Configure(ESideQuestStep InRequiredStep, ESideQuestStep InResultStep,
        const FText& InSubtitleSpeaker = FText::GetEmpty(), const FText& InSubtitleText = FText::GetEmpty());
protected:
    UFUNCTION() void HandleOverlap(AActor* OverlappedActor, AActor* OtherActor);
    UPROPERTY(EditAnywhere, Category="Quest") ESideQuestStep RequiredStep = ESideQuestStep::FollowForestClue;
    UPROPERTY(EditAnywhere, Category="Quest") ESideQuestStep ResultStep = ESideQuestStep::TalkToGoblin;
    UPROPERTY(EditAnywhere, Category="Quest|Subtitle") FText SubtitleSpeaker;
    UPROPERTY(EditAnywhere, Category="Quest|Subtitle", meta=(MultiLine="true")) FText SubtitleText;
};
