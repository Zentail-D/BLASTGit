// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/PatrollingPath.h"

// Sets default values
APatrollingPath::APatrollingPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FVector APatrollingPath::GetPatrolPoint(int const PatrolPointIndex) const
{
	if(PatrolPointIndex>PatrolPoints.Num())
	{
		return PatrolPoints[PatrolPoints.Num()-1];
	}
	if(PatrolPointIndex<0)
	{
		return PatrolPoints[0];
	}
	return PatrolPoints[PatrolPointIndex];
}

int APatrollingPath::NumOfPoint() const
{
	return PatrolPoints.Num();
}

void APatrollingPath::SetPatrolPathPoints(TArray<FVector> NewPatrolPathPoints)
{
	PatrolPoints=NewPatrolPathPoints;
}


