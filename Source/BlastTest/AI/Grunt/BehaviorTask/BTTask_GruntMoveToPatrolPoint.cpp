// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Grunt/BehaviorTask/BTTask_GruntMoveToPatrolPoint.h"

#include "EnemyControllerParent.h"
#include "GruntAIEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Constructor to set up the node name and anything else
UBTTask_GruntMoveToPatrolPoint::UBTTask_GruntMoveToPatrolPoint(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Grunts Move To Patrol Location");
	bNotifyTick = true;
}
	
/** main task function that gets ran from BT */
EBTNodeResult::Type UBTTask_GruntMoveToPatrolPoint::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// set up some critical variables when node is reached
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	// get our controller and character references
	AEnemyControllerParent* const Controller = Cast<AEnemyControllerParent>(owner_comp.GetAIOwner());
	AGruntAIEnemy* GruntAI = Cast<AGruntAIEnemy>(Controller->GetPawn());
	if (GruntAI && Controller)
	{
		// The patrol point we want to move to
		const FVector TargetLocation = Controller->GetBlackboard()->GetValueAsVector(CurrentTargetLocation.SelectedKeyName);

		// must set the max walk speed to use our desired movement speed
		GruntAI->GetCharacterMovement()->MaxWalkSpeed = GruntAI->GetMovementSpeed();
		
		// Call Move to on that point
		Controller->MoveToLocation(TargetLocation, GruntAI->GetMoveToTolerance());
	} else
	{	// set return value to failed and print helpful message
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Controller or Enemy not used correctly in execute task(BTTask_GruntMoveToPatrolPoint.cpp)"));
		NodeResult = EBTNodeResult::Failed;
	}
	return NodeResult;
}

/** what will get called every run through of the behavior tree */
void UBTTask_GruntMoveToPatrolPoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// get our references
	AEnemyControllerParent* const Controller = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	AGruntAIEnemy* GruntAI = Cast<AGruntAIEnemy>(Controller->GetPawn());

	if (GruntAI && Controller)	// check for our controller and character
	{
		// The patrol point we are moving to
		const FVector TargetLocation = Controller->GetBlackboard()->GetValueAsVector(CurrentTargetLocation.SelectedKeyName);
		// Get our current location
		const FVector CurrentLocation = GruntAI->GetTransform().GetLocation();
		// get our distance to location
		const float DistanceToLocation = (TargetLocation - CurrentLocation).Size();
		// if within our move to tolerance plus some extra to account for point being low to player
		if (DistanceToLocation <= GruntAI->GetMoveToTolerance()+100)
		{
			// set our blackboard to get a new patrol point to move to
			Controller->GetBlackboard()->SetValueAsBool(NeedNewPatrolPoint.SelectedKeyName, true);
			
			// Finish our task
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		} else
		{
			// must set the max walk speed to use our desired movement speed
			GruntAI->GetCharacterMovement()->MaxWalkSpeed = GruntAI->GetMovementSpeed();
		
			// Call Move to on that point
			Controller->MoveToLocation(TargetLocation, GruntAI->GetMoveToTolerance());
		}
	} else
	{	// set return value to failed and print helpful message
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Controller or Enemy not used correctly in tick (BTTask_GruntMoveToPatrolPoint.cpp)"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
} 
