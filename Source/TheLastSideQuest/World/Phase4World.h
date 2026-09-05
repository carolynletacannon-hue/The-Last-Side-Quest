#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Phase4World.generated.h"

class AQuestInteractableActor;
class APawn;
class UMaterialInterface;
class UStaticMesh;
class UPrimitiveComponent;

/**
 * Compact source-controlled Phase 4 route. It supplies collision, lighting, gameplay placement and
 * deliberately obvious art hooks; a placed instance may override every presentation class/material.
 */
UCLASS(Blueprintable)
class THELASTSIDEQUEST_API APhase4World : public AActor
{
    GENERATED_BODY()
public:
    APhase4World();
protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art") TObjectPtr<UStaticMesh> GroundMesh;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art") TObjectPtr<UStaticMesh> PropMesh;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art") TObjectPtr<UMaterialInterface> VillageMaterial;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art") TObjectPtr<UMaterialInterface> ForestMaterial;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art") TObjectPtr<UMaterialInterface> RuinsMaterial;

    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Cast") TSubclassOf<AQuestInteractableActor> MildredClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Cast") TSubclassOf<AQuestInteractableActor> GuardClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Cast") TSubclassOf<AQuestInteractableActor> GoblinSurvivorClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Cast") TSubclassOf<AQuestInteractableActor> InscriptionClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Cast") TSubclassOf<AQuestInteractableActor> LeverClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Enemies") TSubclassOf<APawn> GoblinEnemyClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Enemies") TSubclassOf<APawn> SlimeEnemyClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Enemies") TSubclassOf<APawn> SkeletonEnemyClass;

private:
    UFUNCTION() void OpenRuinsGate();
    UPROPERTY() TObjectPtr<UPrimitiveComponent> GateCollision;
    UPROPERTY() TObjectPtr<USceneComponent> GateVisual;
};
