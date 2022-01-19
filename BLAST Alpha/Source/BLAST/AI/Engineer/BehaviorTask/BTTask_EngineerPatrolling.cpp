// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EngineerPatrolling.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_EngineerPatrolling::UBTTask_EngineerPatrolling(FObjectInitializer const& object_initializer)
{
	bNotifyTick=true;
	NodeName=TEXT("Engineer Patrolling");
}

EBTNodeResult::Type UBTTask_EngineerPatrolling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type::InProgress;
}

void UBTTask_EngineerPatrolling::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		Engineer = Cast<AEngineerEnemy>(EnemyController->GetPawn());
		if(Engineer)
		{
			//If the Destination is 0 find a new destination that is random
			if(Engineer->CheckDestinationLocation())
			{
				if(Engineer->GetPatrollingPath())
				{
					FVector FinalLocation = Engineer->GetPatrollingPath()->GetPatrolPoint(Engineer->GetPatrollingPathIndex());
					FinalLocation = Engineer->GetPatrollingPath()->GetActorTransform().TransformPosition(FinalLocation);
					float const X = FMath::FRandRange(-1,1);
					float const Y = FMath::FRandRange(-1,1);
					FVector const DesiredLocation = FinalLocation + FVector(X,Y,0) * OffsetFromPatrolPoint;
					//GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Red,FinalLocation.ToString());
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
					if (NavSys->GetRandomPointInNavigableRadius(DesiredLocation, Engineer->GetPatrolPointRadiusTolerance(), Loc, nullptr))
					{
						Engineer->GetCharacterMovement()->MaxWalkSpeed=Engineer->GetMovementSpeed();
						EnemyController->MoveToLocation(Loc.Location,-1);
						Engineer->SetDestinationLocation(Loc.Location);
					}
				}
				else
				{
					if(GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,TEXT("Didnt Set a patrol path"));
					}
				}
			}
			else
			{
				if(Engineer->bEngineerMoving)
				{
					float ExplosionTime = EnemyController->GetBlackboard()->GetValueAsFloat(NotMovingTimer.SelectedKeyName);
					ExplosionTime-=DeltaSeconds;
					if(ExplosionTime<=0)
					{
						Engineer->EmptyDestinationLocation();
						EnemyController->GetBlackboard()->SetValueAsFloat(NotMovingTimer.SelectedKeyName, Engineer->GetMovingTimer());
						Engineer->bEngineerMoving=false;

					}
					EnemyController->GetBlackboard()->SetValueAsFloat(NotMovingTimer.SelectedKeyName,ExplosionTime);
				}
			}
		}
	}
	if (EnemyController == nullptr || Engineer == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	//Checking to see if we are within the tolerance of end distance
	if(DistanceToFinal()<= Engineer->GetMoveToTolerance())
	{
		Engineer->IncrementPatrollingPathIndex();
		Engineer->EmptyDestinationLocation();
	}
}

float UBTTask_EngineerPatrolling::DistanceToFinal() const
{
	return (Engineer->GetActorLocation()-Engineer->GetDestinationLocation()).Size();
}