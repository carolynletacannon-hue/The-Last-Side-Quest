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
    virtual void DrawHUD() override;
};
