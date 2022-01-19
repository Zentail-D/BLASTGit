// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Flying/BehaviorTask/FindPatrolPath.h"

#include "FlyingEnemyAI.h"
#include "FlyingEnemyAIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

UFindPatrolPath::UFindPatrolPath(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Patrol Path");
}

EBTNodeResult::Type UFindPatrolPath::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AFlyingEnemyAIController* const controller = Cast<AFlyingEnemyAIController>(owner_comp.GetAIOwner());
	if(controller)
	{
		AFlyingEnemyAI* FlyingAI = Cast<AFlyingEnemyAI>(controller->GetPawn());
		if(FlyingAI)
		{
			// set our state to passive so that ai doesnt shoot
			FlyingAI->CurrentAttackState = AFlyingEnemyAI::Passive;

			// set our dampenings since we are in patrolling
			FlyingAI->SetPatrollingDampenings();
			
			// get the current patrol path index from the blackboard and use the index to get the point 
			int const index = controller->GetBlackboard()->GetValueAsInt(CurPatrolPathIndex.SelectedKeyName);
			FVector patrol_path_point = FlyingAI->GetPatrollingPath()->GetPatrolPoint(index);
			patrol_path_point = FlyingAI->GetPatrollingPath()->GetTransform().TransformPosition(patrol_path_point);

			// Set the global point as our target location
			controller->GetBlackboard()->SetValueAsVector(TargetLocation.SelectedKeyName, patrol_path_point);
			
			// since we used the index increment for when we have to use it next time, reset to 0 if necessary
			if (index + 1 >= FlyingAI->GetPatrollingPath()->NumOfPoint())
			{
				controller->GetBlackboard()->SetValueAsInt(CurPatrolPathIndex.SelectedKeyName, 0);
			}else
			{
				controller->GetBlackboard()->SetValueAsInt(CurPatrolPathIndex.SelectedKeyName, index+1);
			}

			/** THIS SECTION IS FOR FINDING AND SETTING THE ACTUAL PATH */
			// setup our start and end and temporary path that FindPath will write the path to
			FVector start = FlyingAI->GetTransform().GetLocation();
			FVector end = patrol_path_point;
			TArray<FVector> path;
			if (FlyingAI->NavVolume3D && FlyingAI->NavVolume3D->FindPath(start, end, object_types, actorclassfilter, path))
			{

				// if find path returns true then we set the neccessary variables in the controller
				controller->PathToTarget = path;
				controller->PathToTargetIndex = 0;
				
				// we just found a new path so lets inform the blackboard of this
				controller->GetBlackboard()->SetValueAsBool(NeedNewPatrolPath.SelectedKeyName, false);

				// so that its known that we need a new attack path
				controller->GetBlackboard()->SetValueAsBool(NeedNewAttackPath.SelectedKeyName, true);

				// Gives the AI its inital push to first point
				FVector FirstNavPathPoint = controller->PathToTarget[0];

				FVector InitialForce = (FirstNavPathPoint - start).GetSafeNormal() * FlyingAI->GetForceToApplyWhenPatrolling();
				FlyingAI->GetCapsuleComponent()->SetSimulatePhysics(false);
				FlyingAI->GetCapsuleComponent()->SetSimulatePhysics(true);
				FlyingAI->GetCapsuleComponent()->SetEnableGravity(false);
				FlyingAI->GetCapsuleComponent()->AddForce(InitialForce, TEXT("None"),true);

				// we will return out of the task now that everything is done
				FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			} else
			{	// somewhat useful debug message
				controller->PathToTargetIndex = -1;	// set to -1 so that it it knows there is no path
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Could not find path in 'FindPatrolPath.cpp' or nav volume not set in flying ai"));
			}
			
		} else
		{	// somewhat useful debug message
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Actor used is not of type 'AFlyingEnemyAI'"));
		}
	} else
	{	// somewhat useful debug message
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Controller used is not of type 'AFlyingEnemyAIController'"));
	}
	
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
