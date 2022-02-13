// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SuiciderPatrolling.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_SuiciderPatrolling::UBTTask_SuiciderPatrolling(FObjectInitializer const& object_initializer)
{
	bNotifyTick=true;
	NodeName=TEXT("Suicider Patrolling");
}

EBTNodeResult::Type UBTTask_SuiciderPatrolling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		SuiciderEnemy = Cast<ASuiciderEnemy>(EnemyController->GetPawn());
		if(SuiciderEnemy)
		{
			//Stop any movement that the suicider was doing beforehand so that we can do our patrolling
			EnemyController->StopMovement();
			if(!MoveToPatrollingPoint())
				return EBTNodeResult::Type::Failed;
				
		}
	}
	return EBTNodeResult::Type::InProgress;
}

void UBTTask_SuiciderPatrolling::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		SuiciderEnemy = Cast<ASuiciderEnemy>(EnemyController->GetPawn());
		if(SuiciderEnemy)
		{
			//If we have reached our desired location we will check to see if we are at the location we want to be at
			if (EnemyController->GetPathFollowingComponent()->DidMoveReachGoal())
			{
				if(!MoveToPatrollingPoint())
					FinishLatentTask(OwnerComp,EBTNodeResult::Type::Failed);
			}
		}
	}
	//Catch any incorrectly cast controller and enemy so that the task will fail and not run for ever
	if (EnemyController == nullptr || SuiciderEnemy == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

bool UBTTask_SuiciderPatrolling::MoveToPatrollingPoint() const
{
	if(SuiciderEnemy || EnemyController)
	{
		if(SuiciderEnemy->GetPatrollingPath())
		{
			//Offset the location around a patrol point so that the enemies do not all try to go to one spot
			FVector FinalLocation = SuiciderEnemy->GetPatrollingPath()->GetPatrolPoint(SuiciderEnemy->GetPatrollingPathIndex());
			FinalLocation = SuiciderEnemy->GetPatrollingPath()->GetActorTransform().TransformPosition(FinalLocation);
			float const X = FMath::FRandRange(-1,1);
			float const Y = FMath::FRandRange(-1,1);
			FVector const DesiredLocation = FinalLocation + FVector(X,Y,0) * OffsetFromPatrolPoint;
			//GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Red,FinalLocation.ToString());
			UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			if(!NavSys)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Nav Mesh Volume not found"));
				return false;
			}
			FNavLocation Loc;
			if (NavSys->GetRandomPointInNavigableRadius(DesiredLocation, SuiciderEnemy->GetMoveToTolerance(), Loc, nullptr))
			{
				SuiciderEnemy->GetCharacterMovement()->MaxWalkSpeed=SuiciderEnemy->GetMovementSpeed();
				EnemyController->MoveToLocation(Loc.Location,-1);
			}
			SuiciderEnemy->IncrementPatrollingPathIndex();
			return true;
		}
		else
		{
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Red, TEXT("Patrolling Path was not set"));
			}
		}
	}
	return false;
}
