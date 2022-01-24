// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DroneFiringPatrol.h"

#include "EnemyControllerParent.h"
#include "EngineerDrones.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

UBTTask_DroneFiringPatrol::UBTTask_DroneFiringPatrol(FObjectInitializer const& object_initializer)
{
	bNotifyTick=true;
	NodeName = TEXT("Drone Firing Patrol");
}

EBTNodeResult::Type UBTTask_DroneFiringPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		EngineerDrone = Cast<AEngineerDrones>(EnemyController->GetPawn());
		if(EngineerDrone)
		{
			//Empty the Destination so that we find a new when we enter this task and save off the firing cooldown to the blackboard
			EnemyController->GetBlackboard()->SetValueAsFloat(FiringCooldown.SelectedKeyName,EngineerDrone->GetAttackingCooldown());
			EngineerDrone->EmptyDestinationLocation();
		}
		return EBTNodeResult::Type::InProgress;
	}
	FinishLatentTask(OwnerComp,EBTNodeResult::Type::Failed);
	return EBTNodeResult::Type::Failed;
}

void UBTTask_DroneFiringPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		EngineerDrone = Cast<AEngineerDrones>(EnemyController->GetPawn());
		if(EngineerDrone)
		{
			//If the firing is 0 then fire and end task
			float FireCooldown = EnemyController->GetBlackboard()->GetValueAsFloat(FiringCooldown.SelectedKeyName);
			FireCooldown-=DeltaSeconds;
			if(FireCooldown<=0)
			{
				EngineerDrone->Fire(EnemyController->GetBlackboard()->GetValueAsInt(TargetCharacterIndex.SelectedKeyName));
				FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
			}
			EnemyController->GetBlackboard()->SetValueAsFloat(FiringCooldown.SelectedKeyName,FireCooldown);
			//if the destination is FVector(0) then find a new location
			if(EngineerDrone->CheckDestinationLocation())
			{
				FindLocationNearBlackBoardKey();
			}
			InitialFinalDistance = DistanceToFinal();
			//Checking to see if we are within the tolerance of end distance
			if(DistanceToFinal()<= EngineerDrone->GetMoveToTolerance())
			{
				//Find a new location
				EngineerDrone->EmptyDestinationLocation();
			}
			//Are we going too fast and need to go slower
			if(BelowMaxVelocity())
			{
				FVector const ForceToAdd = ForceVector() * EngineerDrone->GetAcceleration();
				//IF we are trying to make a huge move then that move was a error and we will prevent it
				if((EngineerDrone->GetActorLocation() +ForceToAdd).Size()>20000)
				{
				}
				else
				{
					EngineerDrone->GetCapsuleComponent()->AddForce(ForceToAdd, TEXT("None"),true);
				}
			}
		}
	}
}

void UBTTask_DroneFiringPatrol::FindLocationNearBlackBoardKey() const
{
	//Instead of return a value to set a blackboard we just skip returning and set the blackboard
	FVector const Destination = EnemyController->GetBlackboard()->GetValueAsVector(DroneInRangeLocation.SelectedKeyName);
	float const X =FMath::FRandRange(-1,1);
	float const Y =FMath::FRandRange(-1,1);
	float const Z =FMath::FRandRange(-1,1);
	FVector const UnitVector = FVector(X,Y,Z);
	FVector const DesiredLocation =UnitVector *EngineerDrone->BoxTriggerBox->GetScaledBoxExtent().Size()/2;
	EngineerDrone->SetDestinationLocation(Destination+DesiredLocation);
}

float UBTTask_DroneFiringPatrol::DistanceToFinal() const
{
	return (EngineerDrone->GetActorLocation()-EngineerDrone->GetDestinationLocation()).Size();
}

FVector UBTTask_DroneFiringPatrol::ForceVector() const
{
	//Lerp the force from direction we want to go to the 0 vector so that the closer we are to the point the smaller force we apply
	FVector UnitVectorDirection = (EngineerDrone->GetDestinationLocation() - EngineerDrone->GetActorLocation()).GetSafeNormal();
	float const AlphaValue = (InitialFinalDistance - DistanceToFinal()) / InitialFinalDistance;
	UnitVectorDirection=UnitVectorDirection + AlphaValue * (FVector(0,0,0) - UnitVectorDirection);
	return UnitVectorDirection;
}

bool UBTTask_DroneFiringPatrol::BelowMaxVelocity() const
{
	UCapsuleComponent* DroneCapsuleComp = EngineerDrone->GetCapsuleComponent();
	float const VectorLength = DroneCapsuleComp->GetPhysicsLinearVelocity(TEXT("None")).Size();
	return VectorLength <= EngineerDrone->GetMovementSpeed();
}
