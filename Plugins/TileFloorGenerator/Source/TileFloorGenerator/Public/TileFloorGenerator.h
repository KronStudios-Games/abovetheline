#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileFloorGenerator.generated.h"

class UStaticMesh;
class UMaterialInterface;
class UStaticMeshComponent;
class USceneComponent;

UCLASS(Blueprintable, meta=(PrioritizeCategories="Tile Grid"))
class TILEFLOORGENERATOR_API ATileFloorGenerator : public AActor
{
	GENERATED_BODY()

public:
	ATileFloorGenerator();

	/** First tile mesh (used where (X+Y) % 2 == 0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid|Meshes")
	TObjectPtr<UStaticMesh> TileA;

	/** Second tile mesh (used where (X+Y) % 2 != 0) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid|Meshes")
	TObjectPtr<UStaticMesh> TileB;

	/** Number of tiles along the X axis */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid|Dimensions", meta=(ClampMin="1", UIMin="1"))
	int32 GridSizeX;

	/** Number of tiles along the Y axis */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid|Dimensions", meta=(ClampMin="1", UIMin="1"))
	int32 GridSizeY;

	/** Spacing between tile centers (X, Y) in world units */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid|Dimensions")
	FVector2D TileSize;

	/** Optional material override for TileA meshes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid|Materials")
	TObjectPtr<UMaterialInterface> TileMaterialA;

	/** Optional material override for TileB meshes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Grid|Materials")
	TObjectPtr<UMaterialInterface> TileMaterialB;

	/** Destroy all existing tile components and regenerate the grid */
	UFUNCTION(BlueprintCallable, Category = "Tile Grid")
	void GenerateTiles();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Tile Grid")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY()
	TArray<TObjectPtr<UStaticMeshComponent>> TileComponents;

	void ClearTiles();

	bool bTilesGenerated;
};
