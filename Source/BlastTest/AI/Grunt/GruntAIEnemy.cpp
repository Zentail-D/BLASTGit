// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Grunt/GruntAIEnemy.h"

AGruntAIEnemy::AGruntAIEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoveToTolerance=200;
	
	// adding muzzle location
	AMuzzleLocation= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleLocation2"));
	AMuzzleLocation->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform);
	AMuzzleLocation->bHiddenInGame=true;
	AMuzzleLocation->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Direct Path to the asset to make it so that the location is visible in the editor
	static ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("StaticMesh'/Game/BLAST/Gameplay/WepSys/Sphere.Sphere'"));
	if(TempMesh.Succeeded())
	{
		AMuzzleLocation->SetStaticMesh(TempMesh.Object);
		AMuzzleLocation->SetRelativeScale3D(FVector(0.09,0.09,0.09));

	}
}

void AGruntAIEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bQueuedFire)
	{
		DecreaseAttackCooldown(DeltaSeconds);
		if(AttackingCooldown<0)
		{
			bQueuedFire=false;
			bReadyToFire=true;
		}
	}
	
	if(bFireBurst)
	{
		if (CalculateBulletsInBurst)	// if we need to find how many bullets for this burst find that number
		{
			BulletsInBurst = FMath::RandRange((int32)MinBulletsToFire, (int32)MaxBulletsToFire);
			CalculateBulletsInBurst = false;
		}
		if (BulletsInBurst <= 0)	// if we fired all bullets in burst then reset appropriate values
		{
			CalculateBulletsInBurst = true;
			ResetAttackCooldown();
			ResetBurstAttackCooldown();
			// reset back to strafing
			NeedNewAttackingDestination = true;
			bFireBurst=false;
		}
		DecreaseAttackCooldown(DeltaSeconds);	// decrease main attack cooldown
		//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FVector(AttackingCooldown).ToString());
		if (AttackingCooldown <= 0.f)		// time to attack
		{
			DecreaseBurstAttackCooldown(DeltaSeconds);	// decrease individual bullet cooldown
			if (TimeBetweenShots <= 0.f)	// time to fire
			{
				GruntAIFire(CurrentTargetPlayerIndex);	// fire bullet at our current target player
				ResetBurstAttackCooldown();		// reset our individual bullet cooldown
				BulletsInBurst -= 1;			// decrement our bullets in burst
			}
		}
	}
}

void AGruntAIEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	/** add our second muzzle location */
	AddMuzzleLocation(AMuzzleLocation);
}

void AGruntAIEnemy::GruntAIFire(int TargetedPlayerIndex)
{
	if (TargetedPlayerIndex>-1)		// if no target or the player list is empty it will be -1
		Fire(TargetedPlayerIndex);
}
