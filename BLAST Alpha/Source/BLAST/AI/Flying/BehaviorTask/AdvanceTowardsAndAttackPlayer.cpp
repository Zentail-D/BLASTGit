// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Flying/BehaviorTask/AdvanceTowardsAndAttackPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

// Constructor to set up the node name and anything else
UAdvanceTowardsAndAttackPlayer::UAdvanceTowardsAndAttackPlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Advance Towards And Attack Player");
	bNotifyTick = true;
}

/** with ticktask in here this will get called once every time the node is visited
*  like a kind of constructor */
EBTNodeResult::Type UAdvanceTowardsAndAttackPlayer::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
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
		for (const FVector point : Controller->PathToTarget)
		{
			DrawDebugCircle(GetWorld(), point, 30, 50, FColor::Red, false, 7.0f, 0, 10);
		}	
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, TEXT("Startng New Path"));
	// Gives the AI its inital push to first point
	if (Controller->PathToTargetIndex != 0)
		return NodeResult;
	const FVector FirstNavPathPoint = Controller->PathToTarget[0];
	const FVector AILocation = FlyingAI->GetActorLocation();
	FVector InitialForce = FVector(0, 0, 0);
	// if we arent on top of the first point then move us to it
	TurnOffPhysics(FlyingAI);
	TurnOnPhysics(FlyingAI);

	if ((FirstNavPathPoint - AILocation).Size() > FlyingAI->GetMoveToTolerance()*2)
	{
		ApplyForceToAI(FirstNavPathPoint, FlyingAI, FColor::Green);
	} else
	{
		FVector SecondNavPathPoint = FirstNavPathPoint;
		if (Controller->PathToTarget.Num()>1)
		{
			SecondNavPathPoint = Controller->PathToTarget[1];
			Controller->PathToTargetIndex += 1;
		}
		ApplyForceToAI(SecondNavPathPoint, FlyingAI, FColor::Green);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, FVector(Controller->PathToTarget.Num()).ToString());
	return NodeResult;
}

/** what will get called every run through of the behavior tree */
void UAdvanceTowardsAndAttackPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// reference for the controller o the ai
	AFlyingEnemyAIController* Controller = Cast<AFlyingEnemyAIController>(OwnerComp.GetAIOwner());
	// reference to the actual AI character being controlled
	AFlyingEnemyAI* FlyingAI = Cast<AFlyingEnemyAI>(Controller->GetPawn());
	
	// if the controller and flyingAI are set
	if (Controller != nullptr && FlyingAI != nullptr)	// only if controller and flyingAI were found and set correctly
	{
		// Find the current target location of our path
		const FVector CurrentTargetLocation = Controller->PathToTarget[Controller->PathToTargetIndex];
		
		if (FlyingAI->ShowDebug)	// only show the velocity when in debug
		{
			const float Velocity = FlyingAI->GetCapsuleComponent()->GetPhysicsLinearVelocity().Size();
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow,
				FString::Printf(TEXT("Current Velocity = %f"), Velocity));
		}
		if (FlyingAI->CanMove())
		{
			// if we are within reach of the target location
			if (CurrentTargetReached(CurrentTargetLocation, FlyingAI))
			{
				// check to see if we are at the end of the navigation path if so then notify the BB and return
				if (Controller->PathToTargetIndex >= Controller->PathToTarget.Num()-1)
				{
					TurnOffPhysics(FlyingAI);
					Controller->GetBlackboard()->SetValueAsBool(NeedNewAttackPath.SelectedKeyName, true);
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}	
				else	// else if we are not at the end of our path
				{
					// increment the current path index so that it continues moving along the navigation path
					// will only happen if ai hasnt reached the end of the navigation path
					Controller->PathToTargetIndex++;
				
					// get our now new target location and apply our force to move in that direction
					const FVector NewTargetLocation = Controller->PathToTarget[Controller->PathToTargetIndex];
					ApplyForceToAI(NewTargetLocation, FlyingAI, FColor::Red);
				}
			}
		} else
		{
			// since we cannot move set out physics to false so that we dont move and stop
			TurnOffPhysics(FlyingAI);
			TurnOnPhysics(FlyingAI);
			if (FlyingAI->CanFireAttack())	// if we are in our fire state then fire
			{
				FlyingAI->FlyingAIFire();	// actually shoot at enemies current target player
				FlyingAI->ResetAttackStateToAgro();		// resets back to agro to start whole process over again
				// since the ai will be stopped we have to actually reapply a force to the target nav point it was going to
				// to get it moving again

				ApplyForceToAI(CurrentTargetLocation, FlyingAI, FColor::Red);
			}
		}
	} else
	{
		// print somewhat useful debug message to help and finish task with a failed result
		GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, TEXT("Controller or FlyingAI not found in 'MoveToTargetLocation.cpp'"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}

void UAdvanceTowardsAndAttackPlayer::ApplyForceToAI(FVector TargetLocation, AFlyingEnemyAI* FlyingAI, FColor color)
{
	// get the current location and use that with the larget location to find which direction will be moved in
	const FVector CurrentLocation = FlyingAI->GetTransform().GetLocation();
	const FVector ForceDirection = (TargetLocation - CurrentLocation).GetSafeNormal();

	if (FlyingAI->ShowDebug)	// if debugging then show the force direction from cur loc. to target
	{
		DrawDebugLine(GetWorld(), CurrentLocation, TargetLocation,
			color, false, 5, 0, 5);
	}

	// if im not simulating physics then simulate it
	if (!FlyingAI->GetCapsuleComponent()->IsSimulatingPhysics())
	{
		TurnOnPhysics(FlyingAI);
	} else
	{
		TurnOffPhysics(FlyingAI);
		TurnOnPhysics(FlyingAI);
	}
	// apply our force with the direction and the max speed of the enemy AI only if we arent charging an attack and can move
	if (FlyingAI->CanMove())
	{
		FlyingAI->GetCapsuleComponent()->AddForce(ForceDirection*FlyingAI->GetForceToApplyWhenAttacking(), TEXT("None"),true);
	}
	
}

bool UAdvanceTowardsAndAttackPlayer::CurrentTargetReached(FVector CurrentTargetLocation, AFlyingEnemyAI* FlyingAI)
{
	// get the current location as well as the distance to the target
	const FVector CurrentLocation = FlyingAI->GetTransform().GetLocation();
	const float DistanceToTarget = (CurrentTargetLocation - CurrentLocation).Size();
	// use the distance to determine if it is less than the move to tolerance, if so return true, else return false
	if (DistanceToTarget <= FlyingAI->GetMoveToTolerance())
	{
		// set simulate physics to false to make the AI full stop
		TurnOffPhysics(FlyingAI);
		return true;
	}
	return false;
}

void UAdvanceTowardsAndAttackPlayer::TurnOffPhysics(const AFlyingEnemyAI* FlyingAI)
{
	FlyingAI->GetCapsuleComponent()->SetSimulatePhysics(false);
	FlyingAI->GetCapsuleComponent()->SetEnableGravity(false);
}

void UAdvanceTowardsAndAttackPlayer::TurnOnPhysics(const AFlyingEnemyAI* FlyingAI)
{
	FlyingAI->GetCapsuleComponent()->SetSimulatePhysics(true);
	FlyingAI->GetCapsuleComponent()->SetEnableGravity(false);
}