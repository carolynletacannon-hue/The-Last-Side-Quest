#include "World/Phase4World.h"

#include "Components/BoxComponent.h"
#include "Boss/MittensBoss.h"
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
#include "Engine/SkyAtmosphere.h"
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
    GroundCollisionMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
}

void APhase4World::BeginPlay()
{
    Super::BeginPlay();
    UWorld* World = GetWorld();
    if (!World) return;

    // A narrow, gently turning 2.5-minute outbound walk: village, forest, then monumental ruins.
    const TArray<FVector> Route = {
        {0,0,-80}, {1400,0,-80}, {2800,120,-80}, {4200,260,-80}, {5600,-100,-80},
        {7000,-240,-80}, {8400,0,-80}, {9800,180,-80}, {11200,180,-80}, {12600,0,-80},
        {14000,0,-80}, {15100,0,-80}, {16200,0,-80}};
    for (int32 Index = 0; Index < Route.Num(); ++Index)
    {
        UStaticMeshComponent* Floor = NewObject<UStaticMeshComponent>(this);
        Floor->RegisterComponent(); Floor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Floor->SetStaticMesh(GroundCollisionMesh); Floor->SetWorldLocation(Route[Index]);
        Floor->SetWorldScale3D(Index >= 10 ? FVector(14,28,.8f) : FVector(14,7,.8f));
        Floor->SetCollisionProfileName(TEXT("BlockAll"));
        UMaterialInterface* AreaMaterial = Index < 3 ? VillageGroundMaterial : (Index < 7 ? ForestGroundMaterial : RuinsGroundMaterial);
        if (AreaMaterial) Floor->SetMaterial(0, AreaMaterial);
    }

    auto SpawnArt = [&](const TCHAR* Prefix, const TArray<TObjectPtr<UStaticMesh>>& Meshes, const TArray<FTransform>& Transforms)
    {
        if (Meshes.IsEmpty()) return;
        for (int32 Index = 0; Index < Transforms.Num(); ++Index)
        {
            UStaticMesh* Mesh = Meshes[Index % Meshes.Num()];
            if (!Mesh) continue;
            UStaticMeshComponent* Art = NewObject<UStaticMeshComponent>(this, *FString::Printf(TEXT("%s_%02d"), Prefix, Index));
            Art->RegisterComponent(); Art->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            Art->SetStaticMesh(Mesh); Art->SetWorldTransform(Transforms[Index]);
            Art->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    };

    // These placements establish composition only. Gameplay collision and actors remain separate below.
    SpawnArt(TEXT("VillageBuilding"), VillageBuildingMeshes, {
        FTransform(FRotator(0,25,0), {450,-750,0}), FTransform(FRotator(0,-20,0), {1050,760,0}),
        FTransform(FRotator(0,18,0), {1850,-760,0}), FTransform(FRotator(0,-30,0), {2700,820,0})});
    SpawnArt(TEXT("VillageProp"), VillageSmallPropMeshes, {
        FTransform(FRotator(0,20,0), {220,380,0}), FTransform(FRotator(0,-35,0), {700,420,0}),
        FTransform(FRotator(0,40,0), {1500,-430,0}), FTransform(FRotator(0,-15,0), {2450,400,0})});
    TArray<FTransform> TreeTransforms;
    for (int32 Index = 3; Index < 8; ++Index) for (const float Side : {-1.f, 1.f})
        TreeTransforms.Emplace(FRotator(0, Index * 37.f + Side * 12.f, 0), Route[Index] + FVector(Index % 2 ? 220.f : -180.f, Side * 650.f, 0), FVector(1.f + (Index % 3) * .12f));
    SpawnArt(TEXT("ForestTree"), TreeMeshes, TreeTransforms);
    SpawnArt(TEXT("ForestRock"), ForestRockMeshes, {
        FTransform(FRotator(0,10,0), {3900,-500,0}, FVector(.8f)), FTransform(FRotator(0,80,0), {5450,520,0}, FVector(1.2f)),
        FTransform(FRotator(0,145,0), {6750,-540,0}, FVector(.9f)), FTransform(FRotator(0,210,0), {8250,590,0}, FVector(1.15f))});
    SpawnArt(TEXT("ForestUndergrowth"), ForestUndergrowthMeshes, {
        FTransform(FRotator(0,0,0), {3600,430,0}), FTransform(FRotator(0,75,0), {4850,-430,0}),
        FTransform(FRotator(0,155,0), {6100,470,0}), FTransform(FRotator(0,235,0), {7350,-460,0}), FTransform(FRotator(0,310,0), {8500,430,0})});
    SpawnArt(TEXT("RuinWall"), RuinWallMeshes, {
        FTransform(FRotator(0,0,0), {9250,-680,0}), FTransform(FRotator(0,180,0), {9250,760,0}),
        FTransform(FRotator(0,0,0), {10800,-720,0}), FTransform(FRotator(0,180,0), {10800,800,0}),
        FTransform(FRotator(0,0,0), {12400,-780,0}, FVector(1.2f)), FTransform(FRotator(0,180,0), {12400,780,0}, FVector(1.2f))});
    SpawnArt(TEXT("RuinArchitecture"), RuinColumnAndArchMeshes, {
        FTransform(FRotator::ZeroRotator, {9000,90,0}), FTransform(FRotator::ZeroRotator, {11050,180,0}, FVector(1.15f)),
        FTransform(FRotator::ZeroRotator, {13200,0,0}, FVector(1.45f))});
    SpawnArt(TEXT("RuinRubble"), RuinRubbleMeshes, {
        FTransform(FRotator(0,20,0), {9550,520,0}), FTransform(FRotator(0,100,0), {10300,-520,0}),
        FTransform(FRotator(0,210,0), {11700,530,0}), FTransform(FRotator(0,300,0), {12800,-560,0})});
    SpawnArt(TEXT("BossFloorArt"), BossFloorMeshes, {FTransform(FRotator::ZeroRotator, {15100,0,5}, FVector(2.2f,2.2f,1))});
    SpawnArt(TEXT("BossColumn"), MonumentalColumnMeshes, {
        FTransform(FRotator::ZeroRotator,{14250,-900,0},FVector(1.5f)), FTransform(FRotator::ZeroRotator,{14250,900,0},FVector(1.5f)),
        FTransform(FRotator::ZeroRotator,{15100,-1050,0},FVector(1.8f)), FTransform(FRotator::ZeroRotator,{15100,1050,0},FVector(1.8f)),
        FTransform(FRotator::ZeroRotator,{16000,-900,0},FVector(2.f)), FTransform(FRotator::ZeroRotator,{16000,900,0},FVector(2.f))});
    SpawnArt(TEXT("BossArch"), BossArchMeshes, {
        FTransform(FRotator::ZeroRotator,{14000,0,0},FVector(1.8f)), FTransform(FRotator::ZeroRotator,{16150,0,0},FVector(2.2f))});
    SpawnArt(TEXT("BossBrazier"), BossBrazierMeshes, {
        FTransform(FRotator::ZeroRotator,{14500,-750,0}), FTransform(FRotator::ZeroRotator,{14500,750,0}),
        FTransform(FRotator::ZeroRotator,{15800,-700,0}), FTransform(FRotator::ZeroRotator,{15800,700,0})});
    SpawnArt(TEXT("BossRubble"), BossRubbleMeshes, {
        FTransform(FRotator(0,30,0),{14700,-1000,0}), FTransform(FRotator(0,150,0),{15300,1000,0})});
    if (ThroneMesh)
        SpawnArt(TEXT("BossThrone"), TArray<TObjectPtr<UStaticMesh>>{ThroneMesh}, {FTransform(FRotator(0,-90,0),{16100,0,40})});

    // Keep a clean clone legible, but never mix obvious primitives into an assigned production-art pass.
    if (VillageBuildingMeshes.IsEmpty() && TreeMeshes.IsEmpty() && RuinColumnAndArchMeshes.IsEmpty())
    {
        UStaticMesh* FallbackProp = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
        for (int32 Index = 0; Index < Route.Num(); ++Index)
        {
            for (const float Side : {-1.f, 1.f})
            {
                UStaticMeshComponent* Landmark = NewObject<UStaticMeshComponent>(this);
                Landmark->RegisterComponent(); Landmark->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
                Landmark->SetStaticMesh(FallbackProp);
                Landmark->SetWorldLocation(Route[Index] + FVector(0, Side * (Index < 3 ? 600 : 520), Index < 7 ? 220 : 350));
                Landmark->SetWorldScale3D(Index < 3 ? FVector(1.4f,1.4f,4) : (Index < 7 ? FVector(2,2,7) : FVector(2.2f,2.2f,10)));
                Landmark->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            }
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
            Placeholder->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"))); Placeholder->SetRelativeLocation({0,0,90});
            Placeholder->SetRelativeScale3D({.55f,.55f,1.8f}); Placeholder->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
        return Actor;
    };
    AQuestInteractableActor* Mildred = SpawnInteractable(MildredClass, {300,180,40}, FText::FromString(TEXT("Talk to Mildred")), ESideQuestStep::TalkToMildred, ESideQuestStep::TalkToGuard, true,
        {Line(TEXT("Mildred"), TEXT("My Mittens has gone missing. Would you find him for me, dear?")), Line(TEXT("Player"), TEXT("I just killed the Dark Lord.")), Line(TEXT("Mildred"), TEXT("That's lovely. Mittens likes chicken.")), Line(TEXT("Quest"), TEXT("The Last Side Quest — Reward: 3 Gold"))});
    Mildred->ConfigureEnding(
        {Line(TEXT("Mildred"), TEXT("Mittens! There you are!")),
         Line(TEXT("Player"), TEXT("That creature has killed at least twelve people.")),
         Line(TEXT("Mildred"), TEXT("Oh, he gets grumpy when he's hungry."))},
        {Line(TEXT("Mildred"), TEXT("Would you mind finding my other cat?")),
         Line(TEXT("Player"), TEXT("No."))});
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
    // Phase 5 owns chamber progression. Entering starts the boss; only defeat unlocks pickup.
    World->SpawnActor<AMittensBoss>(MittensBossClass ? MittensBossClass : AMittensBoss::StaticClass(),
        FVector(15900,0,40), FRotator(0,180,0));

    // Reliable invisible chamber boundaries remain separate from all replaceable cathedral art.
    for (const FVector& WallLocation : TArray<FVector>{{15100,-1400,220},{15100,1400,220},{16300,0,220}})
    {
        UBoxComponent* Wall = NewObject<UBoxComponent>(this);
        Wall->RegisterComponent(); Wall->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Wall->SetWorldLocation(WallLocation);
        Wall->SetBoxExtent(FMath::Abs(WallLocation.Y) > 0 ? FVector(1200,60,300) : FVector(60,1400,300));
        Wall->SetCollisionProfileName(TEXT("BlockAll"));
    }

    // Five restrained forest encounters, then three skeletons after the ruins threshold.
    const TArray<FVector> ForestEnemies = {{4500,120,80},{5200,-220,80},{6250,180,80},{7000,-180,80},{7600,80,80}};
    for (int32 Index = 0; Index < ForestEnemies.Num(); ++Index)
    {
        TSubclassOf<APawn> Class = Index % 2 == 0 ? GoblinEnemyClass : SlimeEnemyClass;
        World->SpawnActor<APawn>(Class ? Class : ASideQuestEnemy::StaticClass(), ForestEnemies[Index], FRotator::ZeroRotator);
    }
    for (const FVector& Location : TArray<FVector>{{9400,120,80},{10100,160,80},{11900,-120,80}})
        World->SpawnActor<APawn>(SkeletonEnemyClass ? SkeletonEnemyClass : ASideQuestEnemy::StaticClass(), Location, FRotator::ZeroRotator);

    // Named environmental-story hooks mark the survivor clearing for the final authored assets.
    const TArray<TTuple<FName, FVector, TObjectPtr<UStaticMesh>>> Clues = {
        {TEXT("DeadGoblinArtHook"), {7800,-260,20}, DeadGoblinMesh}, {TEXT("ScratchedTreeArtHook"), {7920,430,0}, ScratchedTreeMesh},
        {TEXT("DamagedCartArtHook"), {8200,-380,0}, DamagedCartMesh}};
    for (const TTuple<FName, FVector, TObjectPtr<UStaticMesh>>& Clue : Clues)
    {
        UStaticMesh* Mesh = Clue.Get<2>();
        const bool bIsFallback = !Mesh;
        if (bIsFallback) Mesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
        UStaticMeshComponent* Hook = NewObject<UStaticMeshComponent>(this, Clue.Get<0>());
        Hook->RegisterComponent(); Hook->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Hook->SetStaticMesh(Mesh); Hook->SetWorldLocation(Clue.Get<1>());
        if (bIsFallback) Hook->SetWorldScale3D({1.5f,.5f,.35f});
        Hook->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    UBoxComponent* GateBox = NewObject<UBoxComponent>(this, TEXT("RuinsGateCollision"));
    GateBox->RegisterComponent(); GateBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
    GateBox->SetBoxExtent({70,650,300}); GateBox->SetWorldLocation({11100,180,220}); GateBox->SetCollisionProfileName(TEXT("BlockAll"));
    GateCollision = GateBox;
    UStaticMeshComponent* GateMesh = NewObject<UStaticMeshComponent>(this, TEXT("RuinsGateVisual"));
    GateMesh->RegisterComponent(); GateMesh->AttachToComponent(GateBox, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    GateMesh->SetStaticMesh(GateVisualMesh ? GateVisualMesh : GroundCollisionMesh);
    if (!GateVisualMesh) GateMesh->SetRelativeScale3D({0.7f,6.5f,3});
    GateMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); GateVisual = GateMesh;

    const TArray<TPair<FVector, FLinearColor>> Lights = {
        {{900,0,450}, FLinearColor(1,.45f,.16f)}, {{2600,100,450}, FLinearColor(1,.5f,.2f)},
        {{9800,0,350}, FLinearColor(1,.22f,.05f)}, {{11600,0,350}, FLinearColor(1,.18f,.04f)},
        {{14500,-700,380}, FLinearColor(1,.12f,.025f)}, {{14500,700,380}, FLinearColor(1,.12f,.025f)},
        {{15800,-650,420}, FLinearColor(1,.08f,.02f)}, {{15800,650,420}, FLinearColor(1,.08f,.02f)}};
    for (const TPair<FVector, FLinearColor>& Light : Lights)
    {
        APointLight* Actor = World->SpawnActor<APointLight>(Light.Key, FRotator::ZeroRotator);
        Actor->PointLightComponent->SetLightColor(Light.Value); Actor->PointLightComponent->SetIntensity(4500); Actor->PointLightComponent->SetAttenuationRadius(1100);
    }
    ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(FVector::ZeroVector, FRotator(-32,-24,0));
    Sun->GetLightComponent()->SetIntensity(SunIntensity);
    Sun->GetLightComponent()->SetLightColor(SunColor);
    ASkyLight* Sky = World->SpawnActor<ASkyLight>();
    Sky->GetLightComponent()->SetIntensity(SkyLightIntensity);
    Sky->GetLightComponent()->SetLightColor(FLinearColor(.34f,.48f,.62f));
    AExponentialHeightFog* Fog = World->SpawnActor<AExponentialHeightFog>(FVector(7000,0,0), FRotator::ZeroRotator);
    Fog->GetComponent()->SetFogDensity(FogDensity); Fog->GetComponent()->SetVolumetricFog(true);
    World->SpawnActor<ASkyAtmosphere>();

    ANavMeshBoundsVolume* NavBounds = World->SpawnActor<ANavMeshBoundsVolume>(FVector(8200,0,200), FRotator::ZeroRotator);
    if (NavBounds)
    {
        NavBounds->SetActorScale3D({168,24,6});
        if (UNavigationSystemV1* Navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World)) Navigation->OnNavigationBoundsUpdated(NavBounds);
    }
}

void APhase4World::OpenRuinsGate()
{
    if (GateCollision) GateCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (GateVisual) GateVisual->SetVisibility(false, true);
}
