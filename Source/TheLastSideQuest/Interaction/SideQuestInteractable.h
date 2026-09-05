#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SideQuestInteractable.generated.h"

UINTERFACE(BlueprintType)
class USideQuestInteractable : public UInterface { GENERATED_BODY() };

class THELASTSIDEQUEST_API ISideQuestInteractable
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction") FText GetInteractionLabel() const;
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction") bool CanInteract(APawn* Interactor) const;
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction") void Interact(APawn* Interactor);
};
