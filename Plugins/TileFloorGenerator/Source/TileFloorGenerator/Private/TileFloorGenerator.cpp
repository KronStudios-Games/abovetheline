#include "TileFloorGenerator.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"

ATileFloorGenerator::ATileFloorGenerator()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	GridSizeX = 5;
	GridSizeY = 5;
	TileSize = FVector2D(100.0, 100.0);
	TileA = nullptr;
	TileB = nullptr;
	TileMaterialA = nullptr;
	TileMaterialB = nullptr;
	bTilesGenerated = false;
}

void ATileFloorGenerator::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GenerateTiles();
}

void ATileFloorGenerator::BeginPlay()
{
	Super::BeginPlay();

	if (!bTilesGenerated)
	{
		GenerateTiles();
	}
}

void ATileFloorGenerator::ClearTiles()
{
	for (TObjectPtr<UStaticMeshComponent>& Comp : TileComponents)
	{
		if (IsValid(Comp))
		{
			Comp->DestroyComponent();
		}
	}
	TileComponents.Empty();
	bTilesGenerated = false;
}

void ATileFloorGenerator::GenerateTiles()
{
	ClearTiles();

	GridSizeX = FMath::Max(GridSizeX, 1);
	GridSizeY = FMath::Max(GridSizeY, 1);

	if (!TileA && !TileB)
	{
		return;
	}

	TileComponents.Reserve(GridSizeX * GridSizeY);

	for (int32 X = 0; X < GridSizeX; ++X)
	{
		for (int32 Y = 0; Y < GridSizeY; ++Y)
		{
			const bool bIsA = ((X + Y) % 2 == 0);
			UStaticMesh* MeshToUse = bIsA ? TileA.Get() : TileB.Get();

			if (!MeshToUse)
			{
				continue;
			}

			const FName CompName = *FString::Printf(TEXT("Tile_%d_%d"), X, Y);
			UStaticMeshComponent* TileComp = NewObject<UStaticMeshComponent>(this, CompName);

			if (!TileComp)
			{
				continue;
			}

			TileComp->SetStaticMesh(MeshToUse);

			UMaterialInterface* MatOverride = bIsA ? TileMaterialA.Get() : TileMaterialB.Get();
			if (MatOverride)
			{
				TileComp->SetMaterial(0, MatOverride);
			}

			const FVector LocalPosition(
				X * TileSize.X,
				Y * TileSize.Y,
				0.0
			);
			TileComp->SetRelativeLocation(LocalPosition);

			TileComp->SetupAttachment(SceneRoot);
			TileComp->RegisterComponent();

			TileComponents.Add(TileComp);
		}
	}

	bTilesGenerated = true;
}
