// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationNode.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "NavigationVolumeForAI3D.generated.h"

UCLASS()
class BLAST_API ANavigationVolumeForAI3D : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Sets default values for this actor's properties */
	ANavigationVolumeForAI3D();

	private:
	/** The default (root) scene component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationVolume3D", meta = (AllowPrivateAccess = "true"))
	USceneComponent* DefaultSceneComponent = nullptr;

	/** This mesh is what is responsible for showing the grid */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "NavigationVolume3D", meta = (AllowPrivateAccess = "true"))
	UProceduralMeshComponent* ProceduralMesh = nullptr;
	
	/** The number of divisions in the grid along the X axis */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NavigationVolume3D|Pathfinding", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int32 DivisionsX = 10;

	/** The number of divisions in the grid along the Y axis */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NavigationVolume3D|Pathfinding", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int32 DivisionsY = 10;

	/** The number of divisions in the grid along the Z axis */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NavigationVolume3D|Pathfinding", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	int32 DivisionsZ = 10;

	/** The size of each division */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NavigationVolume3D|Pathfinding", meta = (AllowPrivateAccess = "true", ClampMin = 1))
	float DivisionSize = 100.0f;

	/** The minimum number of axes that must be shared with a neighboring node for it to be counted a neighbor */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NavigationVolume3D|Pathfinding", meta = (AllowPrivateAccess = "true", ClampMin = 0, ClampMax = 2))
	int32 MinSharedNeighborAxes = 0;

	/** The thickness of the grid lines */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NavigationVolume3D|Aesthetics", meta = (AllowPrivateAccess = "true", ClampMin = 0))
	float LineThickness = 2.0f;

	/** The color of the grid */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NavigationVolume3D|Aesthetics", meta = (AllowPrivateAccess = "true"))
	FLinearColor Color = FLinearColor(0.0f, 0.0f, 0.0f, 0.5f);

public:
	/**
	* Called when an instance of this class is placed (in editor) or spawned.
	* @param	Transform			The transform the actor was constructed at.
	*/
	virtual void OnConstruction(const FTransform& Transform) override;

	/** Called every frame
	 * @param DeltaTime - time since last frame
	 */
	virtual void Tick(float DeltaTime) override;

	/** Gets the node at the specified coordinates
	 * @param coordinates - will take these coordinates in grid space and use it to find the node releative to those coords
	 * @return NavigationNode* - the navigation node that coorelates to those coordinates
	 */
	NavigationNode* GetNode(FIntVector coordinates) const;

	/** Finds a path from the starting location to the destination
	 * @param start - the location of the start point of the path
	 * @param end - the end location of the path
	 * @param out_path - a reference to a TArray of vectors representing the location in the found path
	 * @return bool - true if path or partial path is found, false if something went very wrong and not path was found
	 */
	UFUNCTION(BlueprintCallable, Category = "NavigationVolume3D")
	bool FindPath(const FVector& start, const FVector& destination, const TArray<TEnumAsByte<EObjectTypeQuery> >& object_types, UClass* actor_class_filter, TArray<FVector>& out_path);

	/**
	* Converts a world space location to a coordinate in the grid. If the location is not located within the grid,
	* the coordinate will be clamped to the closest coordinate.
	* @param	location			The location to convert
	* @return	The converted coordinates
	*/
	UFUNCTION(BlueprintCallable, Category = "NavigationVolume3D")
	UPARAM(DisplayName = "Coordinates") FIntVector ConvertLocationToCoordinates(const FVector& location);

	/**
	* Converts a coordinate into a world space location. If the coordinate is not within the bounds of the grid,
	* the coordinate will be clamped to the closest coordinate.
	* @param	coordinates			The coordinates to convert into world space
	* @return	The converted location in world space
	*/
	UFUNCTION(BlueprintCallable, Category = "NavigationVolume3D")
	UPARAM(DisplayName = "World Location") FVector ConvertCoordinatesToLocation(const FIntVector& coordinates);

	/** Gets the total number of divisions in the grid */
	UFUNCTION(BlueprintPure, Category = "NavigationVolume3D")
	FORCEINLINE int32 GetTotalDivisions() const { return DivisionsX * DivisionsY * DivisionsZ; }

	/** Gets the number of x divisions in the grid */
	UFUNCTION(BlueprintPure, Category = "NavigationVolume3D")
	FORCEINLINE int32 GetDivisionsX() const { return DivisionsX; }

	/** Gets the number of y divisions in the grid */
	UFUNCTION(BlueprintPure, Category = "NavigationVolume3D")
	FORCEINLINE int32 GetDivisionsY() const { return DivisionsY; }

	/** Gets the number of z divisions in the grid */
	UFUNCTION(BlueprintPure, Category = "NavigationVolume3D")
	FORCEINLINE int32 GetDivisionsZ() const { return DivisionsZ; }

	/** Gets the size of each division in the grid */
	UFUNCTION(BlueprintPure, Category = "NavigationVolume3D")
	FORCEINLINE float GetDivisionSize() const { return DivisionSize; }
	
	protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Overridable function called whenever this actor is being removed from a level */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Gets the size of the grid along the X axis */
	inline float GetGridSizeX() const { return DivisionsX * DivisionSize; }

	/** Gets the size of the grid along the Y axis */
	inline float GetGridSizeY() const { return DivisionsY * DivisionSize; }

	/** Gets the size of the grid along the Z axis */
	inline float GetGridSizeZ() const { return DivisionsZ * DivisionSize; }

	private:
	/** Helper function for constructing the grid by creating the line
	 * @param start - where the start point of the line is
	 * @param end - where the end point of the line is
	 * @param normal - the normal of the line
	 * @param vertices - the cumulative array of verts so that the helper can add to it
	 * @param triangles - the array of cumulative triangles that will be used to create mesh
	 */
	void CreateLine(const FVector& start, const FVector& end, const FVector& normal, TArray<FVector>& vertices, TArray<int32>& triangles);

	/** Helper function that checks to make sure coordinates are valid
	 * @param coordinates - the given coordinates to check if valid
	 * @return boolean for if the coords are valid or not
	 */
	bool AreCoordinatesValid(const FIntVector& coordinates) const;

	/** Helper for clamping the coordinates given into a valid set of coordinates
	 * @param coordinates - the coordinates to clamp
	 */
	void ClampCoordinates(FIntVector& coordinates) const;

	/** The nodes used for pathfinding, set to nullptr since its empty */
	NavigationNode* Nodes = nullptr;
};
