// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Flying/FlyingEnemyAI.h"

#include "Components/CapsuleComponent.h"

AFlyingEnemyAI::AFlyingEnemyAI()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// initialize variables
	CurrentAttackChargeUpTime = AttackChargeUpTime;
	CurrentAttackState = AttackingStates::Passive;
	//set enemy type
	this->EnemyType = "fly";
}

void AFlyingEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	SetPatrollingDampenings();
}

void AFlyingEnemyAI::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	switch (CurrentAttackState)
	{
		case AttackingStates::Passive:
			break;
		
		case AttackingStates::Agro:
			// agro logic is: decrease attack cooldown, when it can attack set to charging attack and
			// reset our attacking cooldown for next time
			DecreaseAttackingCooldown(DeltaSeconds);
			if (ShowDebug) // prints the current state and the current attack cooldown
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue, TEXT("Agro"));
				GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue, FVector(AttackingCooldown).ToString());
			}
			if (GetAttackingCooldown() <= 0)
			{
				ResetAttackingCooldown();
				CurrentAttackState = AttackingStates::ChargingAttack;
			}
			break;

		case AttackingStates::ChargingAttack:
			// charging attack logic: when charging the enemy wont be moving so it just sits there
			// until its charging cooldown is over and we reset that while setting our state to CanFire
			DecreaseChargeUpTime(DeltaSeconds);
			if (ShowDebug) // prints the current state and the current charge up timer
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue, TEXT("Charging attack"));
				GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Blue, FVector(CurrentAttackChargeUpTime).ToString());
			}
			if (CurrentAttackChargeUpTime <= 0)
			{
				ResetChargeUpTime();
				CurrentAttackState = AttackingStates::CanFire;
			}
			break;

		case AttackingStates::CanFire:
			// the firing will be called and handled from the AdvanceTowardsAndAttackPlayer.cpp
			break;

		default:	// if we make it to default something is broken with the states, show error
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Attacking State is not valid, (FlyingEnemyAI.cpp)"));
			break;
	}
	
}

void AFlyingEnemyAI::FlyingAIFire()
{
	if (TargetedPlayerIndex>-1)		// TargetedPlayerIndex will be -1 if player list is empty
		Fire(TargetedPlayerIndex);
}

bool AFlyingEnemyAI::CanMove() const
{
	if (CurrentAttackState == AttackingStates::ChargingAttack || CurrentAttackState == AttackingStates::CanFire)
	{
		return false;
	}
	return true;
}

bool AFlyingEnemyAI::CanFireAttack() const
{
	if (CurrentAttackState == AttackingStates::CanFire)
	{
		return true;
	}
	return false;
}

void AFlyingEnemyAI::SetPatrollingDampenings()
{
	GetCapsuleComponent()->SetLinearDamping(PatrollingLinearDampening);
	GetCapsuleComponent()->SetAngularDamping(PatrollingAngularDampening);
}

void AFlyingEnemyAI::SetAttackingDampenings()
{
	GetCapsuleComponent()->SetLinearDamping(AttackingLinearDampening);
	GetCapsuleComponent()->SetAngularDamping(AttackingAngularDampening);
}

