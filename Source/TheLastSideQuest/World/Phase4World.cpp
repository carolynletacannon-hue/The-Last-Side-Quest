#include "World/Phase4World.h"

#include "Components/BoxComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy/SideQuestEnemy.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/DirectionalLight.h"
#include "Engine/PointLight.h"
#include "Engine/SkyLight.h"
#include "Engine/StaticMesh.h"
#include "Interaction/QuestInteractableActor.h"
#include "Materials/MaterialInterface.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Quest/QuestProgressTrigger.h"

namespace
{
FSideQuestDialogueLine Line(const TCHAR* Speaker, const TCHAR* Text)
{
    FSideQuestDialogueLine Result;
    Result.Speaker = FText::FromString(FString(Speaker));
    Result.Text = FText::FromString(FString(Text));
    return Result;
}
}

APhase4World::APhase4World()
{
    PrimaryActorTick.bCanEverTick = false;
    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RouteRoot")));
    GroundMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
    PropMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
}

void APhase4World::BeginPlay()
{
    Super::BeginPlay();
    UWorld* World = GetWorld();
    if (!World) return;

    // A narrow, gently turning 2.5-minute outbound walk: village, forest, then monumental ruins.
    const TArray<FVector> Route = {
        {0,0,-80}, {1400,0,-80}, {2800,120,-80}, {4200,260,-80}, {5600,-100,-80},
        {7000,-240,-80}, {8400,0,-80}, {9800,180,-80}, {11200,180,-80}, {12600,0,-80}, {14000,0,-80}};
    for (int32 Index = 0; Index < Route.Num(); ++Index)
    {
        UStaticMeshComponent* Floor = NewObject<UStaticMeshComponent>(this);
        Floor->RegisterComponent(); Floor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Floor->SetStaticMesh(GroundMesh); Floor->SetWorldLocation(Route[Index]); Floor->SetWorldScale3D({14,7,0.8f});
        Floor->SetCollisionProfileName(TEXT("BlockAll"));
        UMaterialInterface* AreaMaterial = Index < 3 ? VillageMaterial : (Index < 7 ? ForestMaterial : RuinsMaterial);
        if (AreaMaterial) Floor->SetMaterial(0, AreaMaterial);
    }

    // Composition rails double as replacement hooks: village posts, forest trunks, ruin columns.
    for (int32 Index = 0; Index < Route.Num(); ++Index)
    {
        for (const float Side : {-1.0f, 1.0f})
        {
            UStaticMeshComponent* Landmark = NewObject<UStaticMeshComponent>(this);
            Landmark->RegisterComponent(); Landmark->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            Landmark->SetStaticMesh(PropMesh);
            Landmark->SetWorldLocation(Route[Index] + FVector(0, Side * (Index < 3 ? 600 : 520), Index < 7 ? 220 : 350));
            Landmark->SetWorldScale3D(Index < 3 ? FVector(1.4f,1.4f,4) : (Index < 7 ? FVector(2,2,7) : FVector(2.2f,2.2f,10)));
            Landmark->SetCollisionProfileName(TEXT("BlockAll"));
        }
    }

    auto SpawnInteractable = [&](TSubclassOf<AQuestInteractableActor> Class, const FVector& Location, const FText& Label,
        ESideQuestStep Required, ESideQuestStep Result, bool bAdvance, TArray<FSideQuestDialogueLine> Dialogue)
    {
        AQuestInteractableActor* Actor = World->SpawnActor<AQuestInteractableActor>(Class ? Class : AQuestInteractableActor::StaticClass(), Location, FRotator::ZeroRotator);
        Actor->Configure(Label, Required, Result, bAdvance, Dialogue);
        if (!Class)
        {
            UStaticMeshComponent* Placeholder = NewObject<UStaticMeshComponent>(Actor);
            Placeholder->RegisterComponent(); Placeholder->AttachToComponent(Actor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            Placeholder->SetStaticMesh(PropMesh); Placeholder->SetRelativeLocation({0,0,90});
            Placeholder->SetRelativeScale3D({.55f,.55f,1.8f}); Placeholder->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
        return Actor;
    };
    SpawnInteractable(MildredClass, {300,180,40}, FText::FromString(TEXT("Talk to Mildred")), ESideQuestStep::TalkToMildred, ESideQuestStep::TalkToGuard, true,
        {Line(TEXT("Mildred"), TEXT("My Mittens has gone missing. Would you find him for me, dear?")), Line(TEXT("Player"), TEXT("I just killed the Dark Lord.")), Line(TEXT("Mildred"), TEXT("That's lovely. Mittens likes chicken.")), Line(TEXT("Quest"), TEXT("The Last Side Quest — Reward: 3 Gold"))});
    SpawnInteractable(GuardClass, {2100,-120,40}, FText::FromString(TEXT("Talk to Guard")), ESideQuestStep::TalkToGuard, ESideQuestStep::FollowForestClue, true,
        {Line(TEXT("Guard"), TEXT("Hero! The kingdom owes you everything.")), Line(TEXT("Player"), TEXT("Have you seen a cat?")), Line(TEXT("Guard"), TEXT("…Orange?")), Line(TEXT("Player"), TEXT("Yes.")), Line(TEXT("Guard"), TEXT("Went into the forest."))});
    SpawnInteractable(GoblinSurvivorClass, {8050,230,40}, FText::FromString(TEXT("Talk to goblin")), ESideQuestStep::TalkToGoblin, ESideQuestStep::EnterRuins, true,
        {Line(TEXT("Goblin"), TEXT("THE BEAST.")), Line(TEXT("Player"), TEXT("…The cat?")), Line(TEXT("Goblin"), TEXT("DO NOT SPEAK ITS NAME."))});
    SpawnInteractable(InscriptionClass, {9700,-210,80}, FText::FromString(TEXT("Read inscription")), ESideQuestStep::OpenRuinsGate, ESideQuestStep::OpenRuinsGate, false,
        {Line(TEXT("Inscription"), TEXT("HE WHO ENTERS SHALL FACE THE DEVOURER.")), Line(TEXT("Player"), TEXT("Mittens?"))});
    AQuestInteractableActor* Lever = SpawnInteractable(LeverClass, {10500,-250,60}, FText::FromString(TEXT("Pull lever")), ESideQuestStep::OpenRuinsGate, ESideQuestStep::ReachMittens, true, {});
    Lever->OnInteractionFinished.AddDynamic(this, &ThisClass::OpenRuinsGate);

    auto SpawnTrigger = [&](const FVector& Location, ESideQuestStep Required, ESideQuestStep Result)
    {
        AQuestProgressTrigger* Trigger = World->SpawnActor<AQuestProgressTrigger>(Location, FRotator::ZeroRotator);
        Trigger->GetCollisionComponent()->SetWorldScale3D({2,7,3}); Trigger->Configure(Required, Result);
    };
    SpawnTrigger({3400,150,80}, ESideQuestStep::FollowForestClue, ESideQuestStep::TalkToGoblin);
    SpawnTrigger({9000,90,80}, ESideQuestStep::EnterRuins, ESideQuestStep::OpenRuinsGate);
    SpawnTrigger({13400,0,80}, ESideQuestStep::ReachMittens, ESideQuestStep::PickUpMittens);

    // Five restrained forest encounters, then three skeletons after the ruins threshold.
    const TArray<FVector> ForestEnemies = {{4500,120,40},{5200,-220,40},{6250,180,40},{7000,-180,40},{7600,160,40}};
    for (int32 Index = 0; Index < ForestEnemies.Num(); ++Index)
    {
        TSubclassOf<APawn> Class = Index % 2 == 0 ? GoblinEnemyClass : SlimeEnemyClass;
        World->SpawnActor<APawn>(Class ? Class : ASideQuestEnemy::StaticClass(), ForestEnemies[Index], FRotator::ZeroRotator);
    }
    for (const FVector& Location : TArray<FVector>{{9400,120,40},{10100,160,40},{11900,-120,40}})
        World->SpawnActor<APawn>(SkeletonEnemyClass ? SkeletonEnemyClass : ASideQuestEnemy::StaticClass(), Location, FRotator::ZeroRotator);

    // Named environmental-story hooks mark the survivor clearing for the final authored assets.
    const TArray<TPair<FName, FVector>> Clues = {
        {TEXT("DeadGoblinArtHook"), {7800,-260,20}}, {TEXT("ScratchedTreeArtHook"), {7920,430,180}},
        {TEXT("DamagedCartArtHook"), {8200,-380,50}}};
    for (const TPair<FName, FVector>& Clue : Clues)
    {
        UStaticMeshComponent* Hook = NewObject<UStaticMeshComponent>(this, Clue.Key);
        Hook->RegisterComponent(); Hook->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Hook->SetStaticMesh(PropMesh); Hook->SetWorldLocation(Clue.Value); Hook->SetWorldScale3D({1.5f,.5f,.35f});
        Hook->SetCollisionProfileName(TEXT("BlockAll"));
    }

    UBoxComponent* GateBox = NewObject<UBoxComponent>(this, TEXT("RuinsGateCollision"));
    GateBox->RegisterComponent(); GateBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    GateBox->SetBoxExtent({70,650,300}); GateBox->SetWorldLocation({11100,180,220}); GateBox->SetCollisionProfileName(TEXT("BlockAll"));
    GateCollision = GateBox;
    UStaticMeshComponent* GateMesh = NewObject<UStaticMeshComponent>(this, TEXT("RuinsGateVisual"));
    GateMesh->RegisterComponent(); GateMesh->AttachToComponent(GateBox, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    GateMesh->SetStaticMesh(GroundMesh); GateMesh->SetRelativeScale3D({0.7f,6.5f,3}); GateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); GateVisual = GateMesh;

    const TArray<TPair<FVector, FLinearColor>> Lights = {
        {{900,0,450}, FLinearColor(1,.45f,.16f)}, {{2600,100,450}, FLinearColor(1,.5f,.2f)},
        {{9800,0,350}, FLinearColor(1,.22f,.05f)}, {{11600,0,350}, FLinearColor(1,.18f,.04f)}};
    for (const TPair<FVector, FLinearColor>& Light : Lights)
    {
        APointLight* Actor = World->SpawnActor<APointLight>(Light.Key, FRotator::ZeroRotator);
        Actor->PointLightComponent->SetLightColor(Light.Value); Actor->PointLightComponent->SetIntensity(4500); Actor->PointLightComponent->SetAttenuationRadius(1100);
    }
    ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-32,-24,0));
    Sun->GetLightComponent()->SetIntensity(3.2f);
    Sun->GetLightComponent()->SetLightColor(FLinearColor(1.0f,.72f,.48f));
    ASkyLight* Sky = World->SpawnActor<ASkyLight>();
    Sky->GetLightComponent()->SetIntensity(.55f);
    Sky->GetLightComponent()->SetLightColor(FLinearColor(.34f,.48f,.62f));
    AExponentialHeightFog* Fog = World->SpawnActor<AExponentialHeightFog>(FVector(7000,0,0), FRotator::ZeroRotator);
    Fog->GetComponent()->SetFogDensity(.018f); Fog->GetComponent()->SetVolumetricFog(true);

    ANavMeshBoundsVolume* NavBounds = World->SpawnActor<ANavMeshBoundsVolume>(FVector(7300,0,200), FRotator::ZeroRotator);
    NavBounds->SetActorScale3D({150,18,6});
    if (UNavigationSystemV1* Navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World)) Navigation->OnNavigationBoundsUpdated(NavBounds);
}

void APhase4World::OpenRuinsGate()
{
    if (GateCollision) GateCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (GateVisual) GateVisual->SetVisibility(false, true);
}
