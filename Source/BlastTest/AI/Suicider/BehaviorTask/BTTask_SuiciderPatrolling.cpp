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
			EnemyController->GetBlackboard()->SetValueAsFloat(NotMovingTimer.SelectedKeyName,SuiciderEnemy->GetMovingTimer());
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
			//If the Destination is 0 find a new destination that is random
			if(SuiciderEnemy->CheckDestinationLocation())
			{
				if(SuiciderEnemy->GetPatrollingPath())
				{
					EnemyController->GetBlackboard()->SetValueAsFloat(NotMovingTimer.SelectedKeyName, SuiciderEnemy->GetMovingTimer());
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
						FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
					}
					FNavLocation Loc;
					if (NavSys->GetRandomPointInNavigableRadius(DesiredLocation, SuiciderEnemy->GetPatrolPointRadiusTolerance(), Loc, nullptr))
					{
						SuiciderEnemy->GetCharacterMovement()->MaxWalkSpeed=SuiciderEnemy->GetMovementSpeed();
						EnemyController->MoveToLocation(Loc.Location,-1);
						SuiciderEnemy->SetDestinationLocation(Loc.Location);
					}
				}
				else
				{
					if(GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Red, TEXT("Patrolling Path was not set"));
					}
				}
			}
			else
			{
				
				if(SuiciderEnemy->bSuiciderMoving)
				{
					float ExplosionTime = EnemyController->GetBlackboard()->GetValueAsFloat(NotMovingTimer.SelectedKeyName);
					ExplosionTime-=DeltaSeconds;
					if(ExplosionTime<=0)
					{
						SuiciderEnemy->EmptyDestinationLocation();
						EnemyController->GetBlackboard()->SetValueAsFloat(NotMovingTimer.SelectedKeyName, SuiciderEnemy->GetMovingTimer());
						SuiciderEnemy->bSuiciderMoving=false;

					}
					EnemyController->GetBlackboard()->SetValueAsFloat(NotMovingTimer.SelectedKeyName,ExplosionTime);
				}
			}
		}
	}
	if (EnemyController == nullptr || SuiciderEnemy == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	//Checking to see if we are within the tolerance of end distance
	if(DistanceToFinal()<= SuiciderEnemy->GetMoveToTolerance())
	{
		SuiciderEnemy->IncrementPatrollingPathIndex();
		SuiciderEnemy->EmptyDestinationLocation();
	}
}

float UBTTask_SuiciderPatrolling::DistanceToFinal() const
{
	return (SuiciderEnemy->GetActorLocation()-SuiciderEnemy->GetDestinationLocation()).Size();
}
