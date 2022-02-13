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

float ASuiciderEnemy::GetMovingTimer()
{
	return MovingTimer;
}
