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
			//Move around randomly for the engineer
			if (EnemyController->GetPathFollowingComponent()->DidMoveReachGoal())
			{
				FVector const FinalLocation = Engineer->GetActorLocation();
				float const X = FMath::FRandRange(-1,1);
				float const Y = FMath::FRandRange(-1,1);
				FVector const DesiredLocation = FinalLocation + FVector(X,Y,0) * DistanceOfEachPathing;
				UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
				if(!NavSys)
				{
					if(GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Nav Mesh Volume not found"));
					}
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				}
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(DesiredLocation, Engineer->GetMoveToTolerance(), Loc, nullptr))
				{
					Engineer->GetCharacterMovement()->MaxWalkSpeed=Engineer->GetMovementSpeed();
					EnemyController->MoveToLocation(Loc.Location,-1);
				}
				
			}
			
			if (EnemyController == nullptr || Engineer == nullptr)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
	}
}

