// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Flying/BehaviorTask/MoveToTargetLocation.h"

#include "blackboard_keys.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"


UMoveToTargetLocation::UMoveToTargetLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Move To Target Location");
	bNotifyTick = true;
}

EBTNodeResult::Type UMoveToTargetLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// set up some critical variables when node is reached
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	// reference for the controller o the ai
	AFlyingEnemyAIController* Controller = Cast<AFlyingEnemyAIController>(owner_comp.GetAIOwner());

	// this will check to nav path to make sure we have something
	if (Controller->PathToTargetIndex == -1)
	{
		// couldnt find a path or the navvolume3d is not set for flyer
		NodeResult = EBTNodeResult::Failed;
		return NodeResult;
	}
	
	// reference to the actual AI character being controlled
	AFlyingEnemyAI* FlyingAI = Cast<AFlyingEnemyAI>(Controller->GetPawn());

	if (FlyingAI->ShowDebug)	// when in debug draw circle at each point in the current nav path
	{
		for (FVector point : Controller->PathToTarget)
		{
			DrawDebugCircle(GetWorld(), point, 30, 50, FColor::Green, false, 7.0f, 0, 10);
		}	
	}
	
	return NodeResult;
}

void UMoveToTargetLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	// reference for the controller o the ai
	AFlyingEnemyAIController* Controller = Cast<AFlyingEnemyAIController>(OwnerComp.GetAIOwner());
	// reference to the actual AI character being controlled
	AFlyingEnemyAI* FlyingAI = Cast<AFlyingEnemyAI>(Controller->GetPawn());
	
	// if the controller and flyingAI were successfully cast
	if (Controller != nullptr && FlyingAI != nullptr)	// only if controller and flyingAI were found and set correctly
	{
		// Find the current target location of our path
		FVector CurrentTargetLocation = Controller->PathToTarget[Controller->PathToTargetIndex];
		
		if (FlyingAI->ShowDebug)	// only show the velocity when in debug
		{
			float Velocity = FlyingAI->GetCapsuleComponent()->GetPhysicsLinearVelocity().Size();
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow,
				FString::Printf(TEXT("Current Velocity = %f"), Velocity));
		}

		// if we are within reach of the target location
		if (CurrentTargetReached(CurrentTargetLocation, FlyingAI))
		{
			// check to see if we are at the end of the navigation path if so then notify the BB and return
			if (Controller->PathToTargetIndex >= Controller->PathToTarget.Num()-1)
			{
				Controller->GetBlackboard()->SetValueAsBool(NeedNewPatrolPath.SelectedKeyName, true);
				TurnOffPhysics(FlyingAI);
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}	
			else	// else if we are not at the end of our path
			{
				// increment the current path index so that it continues moving along the navigation path
				// will only happen if ai hasnt reached the end of the navigation path
				Controller->PathToTargetIndex++;
				
				// get our now new target location and apply our force to move in that direction
				const FVector NewTargetLocation = Controller->PathToTarget[Controller->PathToTargetIndex];
				ApplyForceToAI(NewTargetLocation, FlyingAI);
			}
		}
	} else
	{
		// print somewhat useful debug message to help and finish task with a failed result
		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, TEXT("Controller or FlyingAI not found in 'MoveToTargetLocation.cpp'"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

void UMoveToTargetLocation::ApplyForceToAI(FVector TargetLocation, AFlyingEnemyAI* FlyingAI)
{
	// get the current location and use that with the larget location to find which direction will be moved in
	FVector CurrentLocation = FlyingAI->GetTransform().GetLocation();
	FVector ForceDirection = (TargetLocation - CurrentLocation).GetSafeNormal();

	if (FlyingAI->ShowDebug)	// if debugging then show the force direction from cur loc. to target
	{
		DrawDebugLine(GetWorld(), CurrentLocation, TargetLocation,
			FColor::Green, false, 5, 0, 5);
	}

	// if im not simulating physics then simulate it
	if (!FlyingAI->GetCapsuleComponent()->IsSimulatingPhysics())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Emerald, TEXT("turing phys back on"));
		TurnOnPhysics(FlyingAI);
	}
	// apply our force with the direction and the max speed of the enemy AI
	FlyingAI->GetCapsuleComponent()->AddForce(ForceDirection*FlyingAI->GetForceToApplyWhenPatrolling(), TEXT("None"),true);
	
}

bool UMoveToTargetLocation::CurrentTargetReached(FVector CurrentTargetLocation, AFlyingEnemyAI* FlyingAI)
{
	// get the current location as well as the distance to the target
	FVector CurrentLocation = FlyingAI->GetTransform().GetLocation();
	float DistanceToTarget = (CurrentTargetLocation - CurrentLocation).Size();
	// use the distance to determine if it is less than the move to tolerance, if so return true, else return false
	if (DistanceToTarget <= FlyingAI->GetMoveToTolerance())
	{
		// set simulate physics to false to make the AI full stop
		TurnOffPhysics(FlyingAI);
		return true;
	}
	return false;
}

void UMoveToTargetLocation::TurnOffPhysics(AFlyingEnemyAI* FlyingAI)
{
	FlyingAI->GetCapsuleComponent()->SetSimulatePhysics(false);
	FlyingAI->GetCapsuleComponent()->SetEnableGravity(false);
}

void UMoveToTargetLocation::TurnOnPhysics(AFlyingEnemyAI* FlyingAI)
{
	FlyingAI->GetCapsuleComponent()->SetSimulatePhysics(true);
	FlyingAI->GetCapsuleComponent()->SetEnableGravity(false);
}



