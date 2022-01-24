// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrollingPath.generated.h"

UCLASS()
class APatrollingPath : public AActor
{
	GENERATED_BODY()
	
	public:	
	// Sets default values for this actor's properties
	APatrollingPath();

	/**
	 *Finds the PatrolPoint at a give index
	 * @param PatrolPointIndex The index in the Array clamped to the size of the array
	 * @return A FVector for the position of the point
	 */
	FVector GetPatrolPoint(int const PatrolPointIndex) const;

	/**
	*Finds the size of the Array of Points
	* @return a int for the number of points in the array
	*/	int NumOfPoint() const;

	/**
	*Set the Patrol Path to another TArray of FVectors
	* @param NewPatrolPathPoints TArray of FVectors for the new patrol points
	*/
	void SetPatrolPathPoints(TArray<FVector> NewPatrolPathPoints);

	private:
	/**Array of patrol points*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (MakeEditWidget = "true", AllowPrivateAccess = "true"))
	TArray<FVector> PatrolPoints;

};
