// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DroneNearPlayer.h"
#include "EngineerDroneSwarm.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_DroneNearPlayer::UBTTask_DroneNearPlayer(FObjectInitializer const& object_initializer)
{
	bNotifyTick=true;
	NodeName=TEXT("Drone Near Player");
}

EBTNodeResult::Type UBTTask_DroneNearPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		EngineerDrone = Cast<AEngineerDrones>(EnemyController->GetPawn());
		if(EngineerDrone)
		{
			//Finding the Location we awant to move to
			FVector const MoveToDest = FindLocationNearPlayer();
			EnemyController->GetBlackboard()->SetValueAsVector(DroneInRangeLocation.SelectedKeyName,MoveToDest);
		}
	}
	return EBTNodeResult::Type::InProgress;
}

void UBTTask_DroneNearPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		EngineerDrone = Cast<AEngineerDrones>(EnemyController->GetPawn());
	}

	if (EnemyController == nullptr || EngineerDrone == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	
	InitialFinalDistance = DistanceToFinal();
	//Checking to see if we are within the tolerance of end distance
	if(DistanceToFinal()<= EngineerDrone->GetMoveToTolerance())
	{
		EnemyController->GetBlackboard()->SetValueAsBool(InRange.SelectedKeyName,true);
		FinishLatentTask(OwnerComp,EBTNodeResult::Type::Succeeded);
	}
	//Are we going too fast and need to go slower
	if(BelowMaxVelocity())
	{
		FVector const ForceToAdd = ForceVector() * EngineerDrone->GetAcceleration();
		//Checking to see if we are trying to do a force vector that is too large it was a error in calculation
		if((EngineerDrone->GetActorLocation() +ForceToAdd).Size()>20000)
		{
		}
		else
		{
			EngineerDrone->GetCapsuleComponent()->AddForce(ForceToAdd, TEXT("None"),true);
		}
	}
}

FVector UBTTask_DroneNearPlayer::FindLocationNearPlayer()
{
	//Calls helper function to find the player
	int const ClosestIndex = FindClosestPlayer();
	if(ClosestIndex>=0)
	{
		//Calls the helper to find the actual location we want to move to
		FVector const Destination =VectorWithDistanceFromPlayer(DistanceFromPlayer, ClosestIndex);
		//Get the drone swarm to call a function in drone swarm that checks to see if we are overlapping a location with the enemy the function will return a new fvector to where we actually are moving to if the other locaiton was bad
		AEngineerDroneSwarm* DroneSwarm = Cast<AEngineerDroneSwarm>(EngineerDrone->GetDroneSwarm());
		if(DroneSwarm)
		{
			return DroneSwarm->PointOverlappingDrones(Destination,EngineerDrone->BoxTriggerBox->GetScaledBoxExtent(),ObjectTypes,ActorClassFilter);
		}
		else
		{
			EngineerDrone->bNeedDestroyed=true;
			return FVector(0);
		}
	}
	else
	{
		EngineerDrone->bNeedDestroyed=true;
		return FVector(0);
	}
}

int UBTTask_DroneNearPlayer::FindClosestPlayer() const
{
	TArray<ACharacter*> Players = EngineerDrone->GetPlayerCharacters();
	//Checking to see if there is a player in the array or at least one if there is one we just check that one if there is two we check both
	if(Players.Num()>0)
	{
		if(Players.Num() >1)
		{
			ACharacter* Player1 = Cast<ACharacter>(Players[0]);
			ACharacter* Player2 = Cast<ACharacter>(Players[1]);

			if(Player1 && Player2)
			{
				FVector Player1Location = Player1->GetActorLocation();
				FVector Player2Location = Player2->GetActorLocation();
				float Player1Distance =(Player1Location-EngineerDrone->GetActorLocation()).Size();
				float Player2Distance =(Player2Location-EngineerDrone->GetActorLocation()).Size();

				if(Player1Distance>Player2Distance)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}
		else
		{
			return 0;
		}
	}
	return -1;
}

FVector UBTTask_DroneNearPlayer::VectorWithDistanceFromPlayer(float Distance, int PlayerIndex) const 
{
	//Find a random location and creates a fvector from that random locaiton and rotate it between -60 and 60 so that it is in front of the player is some way and the z is between 100, and 600
	int const ZAxisRotation =FMath::RandRange(-60,60);
	FVector const UnitVector =(EngineerDrone->GetActorLocation()-EngineerDrone->GetPlayerCharacters()[PlayerIndex]->GetActorLocation()).GetSafeNormal();
	FVector DesiredLocation = UKismetMathLibrary::RotateAngleAxis(UnitVector*Distance,ZAxisRotation,FVector(0,0,1));
	DesiredLocation = EngineerDrone->GetPlayerCharacters()[PlayerIndex]->GetActorLocation() + FVector(DesiredLocation.X,DesiredLocation.Y,FMath::RandRange(100,600));
	return DesiredLocation;
}

float UBTTask_DroneNearPlayer::DistanceToFinal() const
{
	return (EngineerDrone->GetActorLocation()-EnemyController->GetBlackboard()->GetValueAsVector(DroneInRangeLocation.SelectedKeyName)).Size();
}

FVector UBTTask_DroneNearPlayer::ForceVector() const
{

	//Lerp the force from direction we want to go to the 0 vector so that the closer we are to the point the smaller force we apply
	FVector UnitVectorDirection = (EnemyController->GetBlackboard()->GetValueAsVector(DroneInRangeLocation.SelectedKeyName) - EngineerDrone->GetActorLocation()).GetSafeNormal();
	float const AlphaValue = (InitialFinalDistance - DistanceToFinal()) / InitialFinalDistance;
	UnitVectorDirection=UnitVectorDirection + AlphaValue * (FVector(0,0,0) - UnitVectorDirection);
	return UnitVectorDirection;
}

bool UBTTask_DroneNearPlayer::BelowMaxVelocity() const
{
	UCapsuleComponent* DroneCapsuleComp = EngineerDrone->GetCapsuleComponent();
	float const VectorLength = DroneCapsuleComp->GetPhysicsLinearVelocity(TEXT("None")).Size();
	return VectorLength <= EngineerDrone->GetMovementSpeed();
}