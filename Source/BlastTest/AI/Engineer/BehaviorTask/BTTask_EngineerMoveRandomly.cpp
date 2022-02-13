// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EngineerMoveRandomly.h"

#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_EngineerMoveRandomly::UBTTask_EngineerMoveRandomly(FObjectInitializer const& object_initializer)
{
	bNotifyTick=true;
	NodeName=TEXT("Engineer Move Randomly");
}

EBTNodeResult::Type UBTTask_EngineerMoveRandomly::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		Engineer = Cast<AEngineerEnemy>(EnemyController->GetPawn());
		if(Engineer)
		{
			if(!MoveToLocationNearPlayer())
				return EBTNodeResult::Type::Failed;
		}
	}
	return EBTNodeResult::Type::InProgress;
}

void UBTTask_EngineerMoveRandomly::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		Engineer = Cast<AEngineerEnemy>(EnemyController->GetPawn());
		if(Engineer)
		{
			if(EnemyController->GetPathFollowingComponent()->DidMoveReachGoal())
			{
				if(!MoveToLocationNearPlayer())
					FinishLatentTask(OwnerComp,EBTNodeResult::Type::Failed);
			}
		}
	}
}

int UBTTask_EngineerMoveRandomly::FindClosestPlayer() const
{
	TArray<ACharacter*> Players = Engineer->GetPlayerCharacters();
	//Checking to see if there is a player in the array or at least one if there is one we just check that one if there is two we check both
	if(Players.Num()>0)
	{
		if(Players.Num() >1)
		{
			ACharacter* Player1 = Cast<ACharacter>(Players[0]);
			ACharacter* Player2 = Cast<ACharacter>(Players[1]);

			if(Player1 && Player2)
			{
				const FVector Player1Location = Player1->GetActorLocation();
				const FVector Player2Location = Player2->GetActorLocation();
				const float Player1Distance =(Player1Location-Engineer->GetActorLocation()).Size();
				const float Player2Distance =(Player2Location-Engineer->GetActorLocation()).Size();

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

bool UBTTask_EngineerMoveRandomly::MoveToLocationNearPlayer() const
{
	if(Engineer)
	{
		if(EnemyController)
		{
			const int PlayerIndex = FindClosestPlayer();
			if(PlayerIndex>=0)
			{
				//Get the player character and the distance form the engineer
				ACharacter* PlayerCharacter = Engineer->GetPlayerCharacters()[PlayerIndex];
				FVector VectorToPlayer = Engineer->GetActorLocation()- PlayerCharacter->GetActorLocation();

				//Scale that distance to the distance we want from the player and then do a small random rotation to give variety to where the engineer chooses to go
				VectorToPlayer =VectorToPlayer.GetSafeNormal();
				VectorToPlayer = VectorToPlayer * Engineer->GetDistanceFromPlayer();

				VectorToPlayer = VectorToPlayer.RotateAngleAxis(FMath::FRandRange(-25, 25), FVector(0, 0, 1));

				
				// we have final location but we need this to be a move to call with a valid
				UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
				if(!NavSys)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Nav Mesh Volume not found"));
					return false;
				}
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(VectorToPlayer+PlayerCharacter->GetActorLocation(), Engineer->GetMoveToTolerance(), Loc, nullptr))
				{
					Engineer->GetCharacterMovement()->MaxWalkSpeed=Engineer->GetMovementSpeed();
					EnemyController->MoveToLocation(Loc.Location,50);
					return true;
				}
			}
		}
	}
	return false;
}

