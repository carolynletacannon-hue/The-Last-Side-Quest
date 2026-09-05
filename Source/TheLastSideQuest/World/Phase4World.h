#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Phase4World.generated.h"

class AQuestInteractableActor;
class APawn;
class UMaterialInterface;
class UStaticMesh;
class UPrimitiveComponent;
class AMittensBoss;

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

    /** Collision-bearing route floor. Keep this assigned even after decorative roads/terrain are added. */
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Route (Do Not Remove)") TObjectPtr<UStaticMesh> GroundCollisionMesh;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Route (Do Not Remove)") TObjectPtr<UMaterialInterface> VillageGroundMaterial;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Route (Do Not Remove)") TObjectPtr<UMaterialInterface> ForestGroundMaterial;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Route (Do Not Remove)") TObjectPtr<UMaterialInterface> RuinsGroundMaterial;

    /** Optional art is non-colliding; cycle two or three compatible variations to avoid obvious repetition. */
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Village") TArray<TObjectPtr<UStaticMesh>> VillageBuildingMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Village") TArray<TObjectPtr<UStaticMesh>> VillageSmallPropMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Forest") TArray<TObjectPtr<UStaticMesh>> TreeMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Forest") TArray<TObjectPtr<UStaticMesh>> ForestRockMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Forest") TArray<TObjectPtr<UStaticMesh>> ForestUndergrowthMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Ruins") TArray<TObjectPtr<UStaticMesh>> RuinWallMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Ruins") TArray<TObjectPtr<UStaticMesh>> RuinColumnAndArchMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Ruins") TArray<TObjectPtr<UStaticMesh>> RuinRubbleMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Ruins") TObjectPtr<UStaticMesh> GateVisualMesh;
    UPROPERTY(EditDefaultsOnly, Category="Phase 5|Boss Chamber Art") TArray<TObjectPtr<UStaticMesh>> BossFloorMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 5|Boss Chamber Art") TArray<TObjectPtr<UStaticMesh>> MonumentalColumnMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 5|Boss Chamber Art") TArray<TObjectPtr<UStaticMesh>> BossArchMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 5|Boss Chamber Art") TArray<TObjectPtr<UStaticMesh>> BossBrazierMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 5|Boss Chamber Art") TArray<TObjectPtr<UStaticMesh>> BossRubbleMeshes;
    UPROPERTY(EditDefaultsOnly, Category="Phase 5|Boss Chamber Art") TObjectPtr<UStaticMesh> ThroneMesh;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Story") TObjectPtr<UStaticMesh> DeadGoblinMesh;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Story") TObjectPtr<UStaticMesh> ScratchedTreeMesh;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Art|Story") TObjectPtr<UStaticMesh> DamagedCartMesh;

    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Lighting", meta=(ClampMin="0.0")) float SunIntensity = 3.2f;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Lighting") FLinearColor SunColor = FLinearColor(1.0f, .72f, .48f);
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Lighting", meta=(ClampMin="0.0")) float SkyLightIntensity = .55f;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Lighting", meta=(ClampMin="0.0", ClampMax="0.1")) float FogDensity = .018f;

    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Cast") TSubclassOf<AQuestInteractableActor> MildredClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Cast") TSubclassOf<AQuestInteractableActor> GuardClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Cast") TSubclassOf<AQuestInteractableActor> GoblinSurvivorClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Cast") TSubclassOf<AQuestInteractableActor> InscriptionClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Cast") TSubclassOf<AQuestInteractableActor> LeverClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Enemies") TSubclassOf<APawn> GoblinEnemyClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Enemies") TSubclassOf<APawn> SlimeEnemyClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 4|Enemies") TSubclassOf<APawn> SkeletonEnemyClass;
    UPROPERTY(EditDefaultsOnly, Category="Phase 5|Boss") TSubclassOf<AMittensBoss> MittensBossClass;

private:
    UFUNCTION() void OpenRuinsGate();
    UPROPERTY() TObjectPtr<UPrimitiveComponent> GateCollision;
    UPROPERTY() TObjectPtr<USceneComponent> GateVisual;
};
