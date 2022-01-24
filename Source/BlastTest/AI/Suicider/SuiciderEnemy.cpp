// Fill out your copyright notice in the Description page of Project Settings.


#include "SuiciderEnemy.h"

ASuiciderEnemy::ASuiciderEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SuiciderAttackState= ESuiciderAttackEnums::VE_Red;
}

void ASuiciderEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(bNeedDestroyed)
	{
		Destroy();
	}
}

FVector ASuiciderEnemy::GetDestinationLocation() const
{
	return DestinationLocation;
}

void ASuiciderEnemy::SetDestinationLocation(FVector NewLocation)
{
	DestinationLocation=NewLocation;
}

void ASuiciderEnemy::EmptyDestinationLocation()
{
	DestinationLocation =FVector(0);
}

bool ASuiciderEnemy::CheckDestinationLocation() const
{
	if(DestinationLocation == FVector(0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int ASuiciderEnemy::GetPatrollingPathIndex() const
{
	return PatrollingPathIndex;
}

void ASuiciderEnemy::IncrementPatrollingPathIndex()
{
	PatrollingPathIndex++;
	PatrollingPathIndex%=GetPatrollingPath()->NumOfPoint();
	
}

float ASuiciderEnemy::GetExplosionRadius()
{
	return ExplosionRadius;
}

ESuiciderAttackEnums ASuiciderEnemy::GetSuiciderAttackingState()
{
	return SuiciderAttackState;
}

float ASuiciderEnemy::GetPatrolPointRadiusTolerance() const
{
	return PatrolPointRadiusTolerance;
}


float ASuiciderEnemy::GetMovingTimer()
{
	return MovingTimer;
}
