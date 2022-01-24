// Fill out your copyright notice in the Description page of Project Settings.


#include "EngineerTurret.h"
#include "EngineerEnemy.h"

AEngineerTurret::AEngineerTurret()
{
}

float AEngineerTurret::GetDegreeRotation()
{
	return DegreeRotation;
}

float AEngineerTurret::GetTurretPatrolRotationRate()
{
	return TurretPatrolRotationRate;
}

float AEngineerTurret::GetTurretPlayerFoundRotationRate()
{
	return TurretPlayerFoundRotationRate;
}

FRotator AEngineerTurret::GetStartingRotation()
{
	return StartRotation;
}

void AEngineerTurret::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bNeedDestroyed)
	{
		if(EngineerEnemyParent)
		{
			EngineerEnemyParent->SubtractOneToCurrentNumberOfTurret();
		}
		Destroy();
	}
	if(!bDestroyItselfOnEngineerDeath)
	{
		bStartDestroyTimer=true;
	}
	if(bStartDestroyTimer)
	{
		TurretLifeTime-=DeltaSeconds;
	}
	if(TurretLifeTime<=0)
	{
		bNeedDestroyed=true;
	}
}

void AEngineerTurret::SafeToolDestroy()
{
	bStartDestroyTimer=true;
}

void AEngineerTurret::DestroyChildren()
{
	if(EngineerEnemyParent)
	{
		EngineerEnemyParent->SubtractOneToCurrentNumberOfTurret();
	}
}

void AEngineerTurret::BeginPlay()
{
	Super::BeginPlay();
	StartRotation = GetActorRotation();
}
