// Fill out your copyright notice in the Description page of Project Settings.


#include "EngineerDrones.h"
#include "EngineerEnemy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEngineerDrones::AEngineerDrones()
{
	PrimaryActorTick.bCanEverTick = true;
	UCapsuleComponent* MCapsuleComponent = GetCapsuleComponent();
	MCapsuleComponent->SetSimulatePhysics(true);
	MCapsuleComponent->SetEnableGravity(false);
	GetCharacterMovement()->GravityScale=0.0;

	if(!BoxTriggerBox)
	{
		BoxTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTriggerBox"));
		BoxTriggerBox->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform);
	}
	
}

int AEngineerDrones::GetPatrollingPathIndex() const
{
	return PatrollingPathIndex;
}

FVector AEngineerDrones::GetDestinationLocation() const
{
	return DestinationLocation;
}

void AEngineerDrones::SetDestinationLocation(FVector NewLocation)
{
	DestinationLocation = NewLocation;
}

void AEngineerDrones::EmptyDestinationLocation()
{
	DestinationLocation =FVector(0);
}

bool AEngineerDrones::CheckDestinationLocation() const
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

void AEngineerDrones::IncrementPatrollingPathIndex()
{
	PatrollingPathIndex++;
	PatrollingPathIndex%=GetPatrollingPath()->NumOfPoint();
}

float AEngineerDrones::GetRotationAlpha() const
{
	return RotationAlpha;
}

float AEngineerDrones::GetAcceleration() const
{
	return Acceleration;
}

void AEngineerDrones::BeginPlay()
{
	Super::BeginPlay();
	//Needs to be set in Begin Play to take effect
	GetCapsuleComponent()->SetLinearDamping(LinearDamping);
	GetCapsuleComponent()->SetAngularDamping(AngularDamping);
}

AEngineerDroneSwarm* AEngineerDrones::GetDroneSwarm() const
{
	return DroneSwarm;
}

void AEngineerDrones::SetDroneSwarm(AEngineerDroneSwarm* ADroneSwarm)
{
	DroneSwarm = ADroneSwarm;
}

void AEngineerDrones::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bNeedDestroyed)
	{
		if(EngineerEnemyParent)
		{
			EngineerEnemyParent->SubtractOneToCurrentNumberOfDrones();
		}
		Destroy();
	}
	if(!bDestroyItselfOnEngineerDeath)
	{
		bStartDestroyTimer=true;
	}
	if(bStartDestroyTimer)
	{
		DroneLifeTime-=DeltaSeconds;
	}
	if(DroneLifeTime<0)
	{
		bNeedDestroyed=true;
	}
}

void AEngineerDrones::SafeToolDestroy()
{
	bStartDestroyTimer=true;
}

void AEngineerDrones::DestroyChildren()
{
	if(EngineerEnemyParent)
	{
		EngineerEnemyParent->SubtractOneToCurrentNumberOfDrones();
	}
}
