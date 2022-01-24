// Fill out your copyright notice in the Description page of Project Settings.


#include "NavigationVolumeForAI3D.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include <set>
#include <unordered_map>

static UMaterial* GridMaterial = nullptr;

// Sets default values
ANavigationVolumeForAI3D::ANavigationVolumeForAI3D()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the default scene component
	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneComponent");
	SetRootComponent(DefaultSceneComponent);

	// Create the procedural mesh component
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());
	ProceduralMesh->CastShadow = false;
	ProceduralMesh->SetEnableGravity(false);
	ProceduralMesh->bApplyImpulseOnDamage = false;
	ProceduralMesh->SetGenerateOverlapEvents(false);
	ProceduralMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	ProceduralMesh->SetCollisionProfileName("NoCollision");
	ProceduralMesh->bHiddenInGame = false;

	// By default, hide the volume while the game is running
	SetActorHiddenInGame(true);

	// Find and save the grid material
	static ConstructorHelpers::FObjectFinder<UMaterial> materialFinder(TEXT("Material'/Game/BLAST/EnvArt/Res/Mat/GridMat/M_Grid_parent.M_Grid_parent'"));
	GridMaterial = materialFinder.Object;

}

void ANavigationVolumeForAI3D::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Create arrays to store the vertices and the triangles that will eventually be uses to create the mesh
	TArray<FVector> vertices;
	TArray<int32> triangles;
	
	// Define variables for the start and end of the line, these will be reused for each line creation
	FVector start = FVector::ZeroVector;
	FVector end = FVector::ZeroVector;

	// Create the X lines geometry
	for (int32 z = 0; z <= DivisionsZ; ++z)
	{
		start.Z = end.Z = z * DivisionSize;
		for (int32 x = 0; x <= DivisionsX; ++x)
		{
			start.X = end.X = (x * DivisionSize);
			end.Y = GetGridSizeY();
			// since these lines are horizontal only the up vector is the normal here
			CreateLine(start, end, FVector::UpVector, vertices, triangles);
		}
	}

	// Reset start and end variables
	start = FVector::ZeroVector;
	end = FVector::ZeroVector;

	// Create the Y lines geometry
	for (int32 z = 0; z <= DivisionsZ; ++z)
	{
		start.Z = end.Z = z * DivisionSize;
		for (int32 y = 0; y <= DivisionsY; ++y)
		{
			start.Y = end.Y = (y * DivisionSize);
			end.X = GetGridSizeX();

			CreateLine(start, end, FVector::UpVector, vertices, triangles);
		}
	}

	// Reset start and end variables
	start = FVector::ZeroVector;
	end = FVector::ZeroVector;

	// Create the Z lines geometry
	for (int32 x = 0; x <= DivisionsX; ++x)
	{
		start.X = end.X = x * DivisionSize;
		for (int32 y = 0; y <= DivisionsY; ++y)
		{
			start.Y = end.Y = (y * DivisionSize);
			end.Z = GetGridSizeZ();

			CreateLine(start, end, FVector::ForwardVector, vertices, triangles);
		}
	}

	// Unused variables that are required to be passed to CreateMeshSection
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> Colors;
	TArray<FProcMeshTangent> Tangents;

	// AGive createmesh section everything worked on in this function to create the actual mesh
	ProceduralMesh->CreateMeshSection(0, vertices, triangles, Normals, UVs, Colors, Tangents, false);

	// Set the material on the procedural mesh so it's color/opacity can be configurable
	UMaterialInstanceDynamic* dynamicMaterialInstance = UMaterialInstanceDynamic::Create(GridMaterial, this);
	dynamicMaterialInstance->SetVectorParameterValue("Color", Color);
	dynamicMaterialInstance->SetScalarParameterValue("Opacity", Color.A);
	ProceduralMesh->SetMaterial(0, dynamicMaterialInstance);
}

// Called every frame
void ANavigationVolumeForAI3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

NavigationNode* ANavigationVolumeForAI3D::GetNode(FIntVector coordinates) const
{
	// clamp the coordinates first in case they're out of range
	ClampCoordinates(coordinates);

	// calculate the index into the node array to get the node wanted
	const int32 divisionPerLevel = DivisionsX * DivisionsY;
	const int32 index = (coordinates.Z * divisionPerLevel) + (coordinates.Y * DivisionsX) + coordinates.X;
	return &Nodes[index];
}

bool ANavigationVolumeForAI3D::FindPath(const FVector& start, const FVector& destination, const TArray<TEnumAsByte<EObjectTypeQuery> >& object_types, UClass* actor_class_filter, TArray<FVector>& out_path)
{
	// Clear the out path
	out_path.Empty();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
	//	FString::Printf(TEXT("find path")));
	// set for the incomplete path
	TArray<FVector> myIncompletePath;
	// NodeCompare is what is used to sort this multiset from lowest FScore to highest FScore
	std::multiset<NavigationNode*, NodeCompare> openSet;
	// cameFrom is a map of nodes that indicate <Node that was visited next , Current node>
	std::unordered_map<NavigationNode*, NavigationNode*> cameFrom;
	// this is a node and that nodes distance to the end node
	std::unordered_map<NavigationNode*, float> gScores;

	NavigationNode* startNode = GetNode(ConvertLocationToCoordinates(start));
	NavigationNode* endNode = GetNode(ConvertLocationToCoordinates(destination));

	// lambda functions that are used within this funciton as helpers
	// h is used to find FScores, basically just distance from "node" to end Node
	auto h = [endNode](NavigationNode* node)
	{
		return FVector::Distance(FVector(endNode->Coordinates), FVector(node->Coordinates));
	};
	// this is just the distance between node1 and node2
	auto distance = [](NavigationNode* node1, NavigationNode* node2)
	{
		return FVector::Distance(FVector(node1->Coordinates), FVector(node2->Coordinates));
	};
	// finds the gScore of the given node from the gScores map
	auto gScore = [&gScores](NavigationNode* node)
	{
		auto iter = gScores.find(node);
		if (iter != gScores.end())
			return iter->second;
		return FLT_MAX;
	};

	startNode->FScore = h(startNode);
	openSet.insert(startNode);
	gScores[startNode] = 0.0f;

	// main loop for finding a complete path
	while (openSet.empty() == false)
	{
		NavigationNode* current = *openSet.begin();
		
		if (current == endNode)		// if we've reached the end then rebuild path
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
			//FString::Printf(TEXT("came from length = %d"), (int)cameFrom.size()));
			out_path.Add(ConvertCoordinatesToLocation(current->Coordinates));

			while (true)
			{
				auto iter = cameFrom.find(current);
				if (iter != cameFrom.end())
				{
					current = iter->second;
					out_path.Insert(ConvertCoordinatesToLocation(current->Coordinates), 0);
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
					//	FString::Printf(TEXT("complete path point: %f %f %f"), node_location.X, node_location.Y, node_location.Z));
				}
				else
				{
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
				    //FString::Printf(TEXT("length of complete path = %d"), (int)out_path.Num()));
					return true;
				}
			}
		}

		openSet.erase(openSet.begin());
		//from what i can tell this for loop bascially loops through all the neightbors of the
		//the current node and weights them based on their distance to the end
		for (NavigationNode* neighbor : current->Neighbors)
		{
			// this gScore is representative of the "greatest" score
			const float tentative_gScore = gScore(current) + distance(current, neighbor);
			// trying to find the best scores, aka go in the direction of the end node
			if (tentative_gScore < gScore(neighbor))
			{
				TArray<AActor*> outActors;
				const FVector worldLocation = ConvertCoordinatesToLocation(neighbor->Coordinates);
				// checks to see if the next node being checked is inside of something
				bool traversable = !(UKismetSystemLibrary::BoxOverlapActors(GWorld, worldLocation, FVector(GetDivisionSize() / 2.0f), object_types, actor_class_filter, TArray<AActor*>(), outActors));
				if (traversable == true)
				{
					cameFrom[neighbor] = current;
					gScores[neighbor] = tentative_gScore;
					const float fScore = tentative_gScore + h(neighbor);
					neighbor->FScore = fScore;
					openSet.insert(neighbor);
				}
			}
		}
	}
	// this section is dedicated to creating the incomplete path for the AI if it cant reach its goal
	if (!cameFrom.empty())
	{
		// this uses basically the same algorithm for creating the out_path as is used when the
		// end point is reached
		auto current_it = cameFrom.begin();
		NavigationNode* current = current_it->first;
		out_path.Add(ConvertCoordinatesToLocation(current->Coordinates));
		while (true)
		{
			
			auto iter = cameFrom.find(current);
			if (iter != cameFrom.end())
			{
				current = iter->second;
				out_path.Insert(ConvertCoordinatesToLocation(current->Coordinates), 0);
			}
			else
			{
				return true;	//return true since we created a partial path
			}
		}
	}
	// Failed to find path
	return false;
}

FIntVector ANavigationVolumeForAI3D::ConvertLocationToCoordinates(const FVector& location)
{
	// coordinates that we'll be returning
	FIntVector coordinates;

	// Convert the location into grid space
	const FVector gridSpaceLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), location);

	// Convert the grid space location to a coordinate (x,y,z)
	coordinates.X = DivisionsX * (gridSpaceLocation.X / GetGridSizeX());
	coordinates.Y = DivisionsY * (gridSpaceLocation.Y / GetGridSizeY());
	coordinates.Z = DivisionsZ * (gridSpaceLocation.Z / GetGridSizeZ());

	// clamp the coords just to be safe
	ClampCoordinates(coordinates);
	return coordinates;
}

FVector ANavigationVolumeForAI3D::ConvertCoordinatesToLocation(const FIntVector& coordinates)
{
	// clamp the cords just in case we get passed unclapped cords
	FIntVector clampedCoordinates(coordinates);
	ClampCoordinates(clampedCoordinates);

	// Convert the coordinates into a grid space location
	FVector gridSpaceLocation = FVector::ZeroVector;
	gridSpaceLocation.X = (clampedCoordinates.X * DivisionSize) + (DivisionSize * 0.5f);
	gridSpaceLocation.Y = (clampedCoordinates.Y * DivisionSize) + (DivisionSize * 0.5f);
	gridSpaceLocation.Z = (clampedCoordinates.Z * DivisionSize) + (DivisionSize * 0.5f);

	// Convert the grid space location into world space
	return UKismetMathLibrary::TransformLocation(GetActorTransform(), gridSpaceLocation);
}

// Called when the game starts or when spawned
void ANavigationVolumeForAI3D::BeginPlay()
{
	Super::BeginPlay();

	/** this will set up all the neighbors for all the nodes */

	// Allocate nodes used for pathfinding
	Nodes = new NavigationNode[GetTotalDivisions()];

	// Helper lambda for adding a neighbor
	auto addNeighborIfValid = [&](NavigationNode* node, const FIntVector& neighbor_coordinates)
	{
		// Make sure the neighboring coordinates are valid
		if (AreCoordinatesValid(neighbor_coordinates))
		{
			int32 sharedAxes = 0;
			if (node->Coordinates.X == neighbor_coordinates.X)
				++sharedAxes;
			if (node->Coordinates.Y == neighbor_coordinates.Y)
				++sharedAxes;
			if (node->Coordinates.Z == neighbor_coordinates.Z)
				++sharedAxes;
		
			// Only add the neighbor if we share more axes with it then the required min shared neighbor axes and it isn't the same node as us
			if (sharedAxes >= MinSharedNeighborAxes && sharedAxes < 3)
			{
				node->Neighbors.push_back(GetNode(neighbor_coordinates));
			}
		}
	};

	// For each node, find its neighbors and assign its coordinates
	for (int32 z = 0; z < DivisionsZ; ++z)
	{
		for (int32 y = 0; y < DivisionsY; ++y)
		{
			for (int32 x = 0; x < DivisionsX; ++x)
			{
				NavigationNode* node = GetNode(FIntVector(x, y, z));
				node->Coordinates = FIntVector(x, y, z);

				// Above neighbors
				{
					// front
					{
						addNeighborIfValid(node, FIntVector(x + 1, y - 1, z + 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 0, z + 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 1, z + 1));
					}
					// middle
					{
						addNeighborIfValid(node, FIntVector(x + 0, y - 1, z + 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 0, z + 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 1, z + 1));
					}
					// back
					{
						addNeighborIfValid(node, FIntVector(x - 1, y - 1, z + 1));
						addNeighborIfValid(node, FIntVector(x - 1, y + 0, z + 1));
						addNeighborIfValid(node, FIntVector(x - 1, y + 1, z + 1));
					}
				}

				// Middle neighbors
				{
					// front
					{
						addNeighborIfValid(node, FIntVector(x + 1, y - 1, z + 0));
						addNeighborIfValid(node, FIntVector(x + 1, y + 0, z + 0));
						addNeighborIfValid(node, FIntVector(x + 1, y + 1, z + 0));
					}
					// middle
					{
						addNeighborIfValid(node, FIntVector(x + 0, y - 1, z + 0));
						addNeighborIfValid(node, FIntVector(x + 0, y + 1, z + 0));
					}
					// back
					{
						addNeighborIfValid(node, FIntVector(x - 1, y - 1, z + 0));
						addNeighborIfValid(node, FIntVector(x - 1, y + 0, z + 0));
						addNeighborIfValid(node, FIntVector(x - 1, y + 1, z + 0));
					}
				}

				// Below neighbors
				{
					// front
					{
						addNeighborIfValid(node, FIntVector(x + 1, y - 1, z - 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 0, z - 1));
						addNeighborIfValid(node, FIntVector(x + 1, y + 1, z - 1));
					}
					// middle
					{
						addNeighborIfValid(node, FIntVector(x + 0, y - 1, z - 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 0, z - 1));
						addNeighborIfValid(node, FIntVector(x + 0, y + 1, z - 1));
					}
					// back
					{
						addNeighborIfValid(node, FIntVector(x - 1, y - 1, z - 1));
						addNeighborIfValid(node, FIntVector(x - 1, y + 0, z - 1));
						addNeighborIfValid(node, FIntVector(x - 1, y + 1, z - 1));
					}
				}
			}
		}
	}
	
}

void ANavigationVolumeForAI3D::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Delete the nodes
	delete [] Nodes;
	Nodes = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ANavigationVolumeForAI3D::CreateLine(const FVector& start, const FVector& end, const FVector& normal, TArray<FVector>& vertices, TArray<int32>& triangles)
{
	// Calculate the half line thickness and the thickness direction
	float halfLineThickness = LineThickness / 2.0f;
	FVector line = end - start;
	line.Normalize();

	// 0--------------------------1
	// |          line		      |
	// 2--------------------------3
	auto createFlatLine = [&](const FVector& thicknessDirection)
	{
		// Top triangle
		triangles.Add(vertices.Num() + 2);
		triangles.Add(vertices.Num() + 1);
		triangles.Add(vertices.Num() + 0);

		// Bottom triangle
		triangles.Add(vertices.Num() + 2);
		triangles.Add(vertices.Num() + 3);
		triangles.Add(vertices.Num() + 1);

		// Vertex 0
		vertices.Add(start + (thicknessDirection * halfLineThickness));
		// Vertex 1
		vertices.Add(end + (thicknessDirection * halfLineThickness));
		// Vertex 2
		vertices.Add(start - (thicknessDirection * halfLineThickness));
		// Vertex 3
		vertices.Add(end - (thicknessDirection * halfLineThickness));
	};

	FVector direction1 = UKismetMathLibrary::Cross_VectorVector(line, normal);
	FVector direction2 = UKismetMathLibrary::Cross_VectorVector(line, direction1);

	createFlatLine(direction1);
	createFlatLine(direction2);
}

bool ANavigationVolumeForAI3D::AreCoordinatesValid(const FIntVector& coordinates) const
{
	return coordinates.X >= 0 && coordinates.X < DivisionsX &&
		coordinates.Y >= 0 && coordinates.Y < DivisionsY &&
		coordinates.Z >= 0 && coordinates.Z < DivisionsZ;
}

void ANavigationVolumeForAI3D::ClampCoordinates(FIntVector& coordinates) const
{
	coordinates.X = FMath::Clamp(coordinates.X, 0, DivisionsX - 1);
	coordinates.Y = FMath::Clamp(coordinates.Y, 0, DivisionsY - 1);
	coordinates.Z = FMath::Clamp(coordinates.Z, 0, DivisionsZ - 1);
}

