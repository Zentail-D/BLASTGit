// Fill out your copyright notice in the Description page of Project Settings.


#include "EngineerToolsParent.h"

// Sets default values
AEngineerToolsParent::AEngineerToolsParent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEngineerToolsParent::SafeToolDestroy()
{
}

void AEngineerToolsParent::SetEngineerParent(AEngineerEnemy* EngineerEnemy)
{
	EngineerEnemyParent=EngineerEnemy;
}
