// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Grunt/BehaviorTask/BTTask_GruntFindPatrolPathPoint.h"

#include "DrawDebugHelpers.h"
#include "EnemyControllerParent.h"
#include "GruntAIEnemy.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GruntFindPatrolPathPoint::UBTTask_GruntFindPatrolPathPoint(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("GruntsFindPatrolPathPoint");
}
	
/** main task function that gets ran from BT */
EBTNodeResult::Type UBTTask_GruntFindPatrolPathPoint::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AEnemyControllerParent* const Controller = Cast<AEnemyControllerParent>(owner_comp.GetAIOwner());
	if(Controller)
	{
		AGruntAIEnemy* GruntAI = Cast<AGruntAIEnemy>(Controller->GetPawn());
		if(GruntAI)
		{
			// get the navigation system and generate a random location on the NavMesh
			UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			if(!NavSys)	// check to see if the nav sys was found, if not return false
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Nav Mesh Volume not found"));
				FinishLatentTask(owner_comp, EBTNodeResult::Failed);
				return EBTNodeResult::Failed;
			}
			
			// get the current patrol path index from the blackboard and use the index to get the point 
			int Index = Controller->GetBlackboard()->GetValueAsInt(CurPatrolPathIndex.SelectedKeyName);
			FVector PatrolPathPoint = GruntAI->GetPatrollingPath()->GetPatrolPoint(Index);
			PatrolPathPoint = GruntAI->GetPatrollingPath()->GetTransform().TransformPosition(PatrolPathPoint);
			
			
			// use the world point to get a navigable point on mesh
			FNavLocation Loc;
			if (NavSys->GetRandomPointInNavigableRadius(PatrolPathPoint, GruntAI->GetPatrolPointRadiusTolerance(), Loc, nullptr))
			{
				// sets the target location as the navigation location found
				Controller->GetBlackboard()->SetValueAsVector(TargetLocation.SelectedKeyName, Loc.Location);
				
				// also update the index (be sure to wrap back to the beginning
				int PatrolPoints = GruntAI->GetPatrollingPath()->NumOfPoint();
				if (Index >= PatrolPoints - 1)
				{
					Controller->GetBlackboard()->SetValueAsInt(CurPatrolPathIndex.SelectedKeyName, 0);
				} else
				{
					Controller->GetBlackboard()->SetValueAsInt(CurPatrolPathIndex.SelectedKeyName, Index + 1);
				}
				
				// we no longer need a a patrol point so we set this so that we can start moving there
				Controller->GetBlackboard()->SetValueAsBool(NeedNewPatrolPoint.SelectedKeyName, false);
				if (GruntAI->ShowDebug)
					DrawDebugCircle(GetWorld(), Loc.Location, 20, 50, FColor::Red, false, 7.0f, 0, 10);
				// all done :)
				FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			} else
            {	// we couldn't get a nav point mesh
                FinishLatentTask(owner_comp, EBTNodeResult::Failed);
                return EBTNodeResult::Failed;
            }
		}
		else
		{	// we couldnt get the character so we print that and return failed
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Enemy of wrong type for this task"));
			FinishLatentTask(owner_comp, EBTNodeResult::Failed);
			return EBTNodeResult::Failed;
		}
	} else
	{	// we couldnt get the controller so we print that and return failed
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Controller of wrong type for this task"));
		FinishLatentTask(owner_comp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	// all done :)
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}