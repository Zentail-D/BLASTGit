// Fill out your copyright notice in the Description page of Project Settings.


#include "EngineerMine.h"
#include "EngineerEnemy.h"

AEngineerMine::AEngineerMine() 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEngineerMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!bNeverDestroy)
	{
		if(bNeedDestroyed)
		{
			if(EngineerEnemyParent)
			{
				EngineerEnemyParent->SubtractOneToCurrentNumberOfMines();
			}
			Destroy();
		}
	}
	if(!bDestroyItselfOnEngineerDeath)
	{
		bStartDestroyTimer=true;
	}
	if(bStartDestroyTimer)
	{
		MineLifeTime-=DeltaTime;
	}
	if(MineLifeTime<=0)
	{
		bNeedDestroyed=true;
	}
}

void AEngineerMine::SafeToolDestroy()
{
	bStartDestroyTimer=true;
}

void AEngineerMine::DestroyChildren()
{
	if(EngineerEnemyParent)
	{
		EngineerEnemyParent->SubtractOneToCurrentNumberOfMines();
	}
}

