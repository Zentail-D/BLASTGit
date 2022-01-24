// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Grunt/BehaviorTask/BTTask_GruntPlayerFoundChasing.h"

#include "DrawDebugHelpers.h"
#include "EnemyControllerParent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

class AGruntAIEnemy;
// Constructor to set up the node name and anything else
UBTTask_GruntPlayerFoundChasing::UBTTask_GruntPlayerFoundChasing(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Grunts Player Found Chasing");
	bNotifyTick = true;
}
	
/** main task function that gets ran from BT */
EBTNodeResult::Type UBTTask_GruntPlayerFoundChasing::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// set up some critical variables when node is reached
	EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;

	// initialize the controller and character
	AEnemyControllerParent* const Controller = Cast<AEnemyControllerParent>(owner_comp.GetAIOwner());
	AGruntAIEnemy* GruntAI = Cast<AGruntAIEnemy>(Controller->GetPawn());
	if (!Controller || !GruntAI)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Incorrect Controller or character being used with node 'GruntPlayerFoundChasing.cpp"));
		FinishLatentTask(owner_comp, EBTNodeResult::Failed);
	}
	// get our array of players
	const TArray<ACharacter*> Players = GruntAI->GetPlayerCharacters();

	// Right when we get to the node set a target for when we approach
	FindNewTarget(Players, GruntAI);
	// since we will start approaching set that attack state
	GruntAI->SetAttackingState(AGruntAIEnemy::Approaching);

	// also set the need new patrol point if the player gets out of sight of enemy
	Controller->GetBlackboard()->SetValueAsBool(NeedNewPatrolPoint.SelectedKeyName, true);
	
	return NodeResult;
} 

/** what will get called every run through of the behavior tree */
void UBTTask_GruntPlayerFoundChasing::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	// initialize the controller and character
	AEnemyControllerParent* const Controller = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	AGruntAIEnemy* GruntAI = Cast<AGruntAIEnemy>(Controller->GetPawn());
	// check if we got the right ones, if not print message and return failed
	if (!Controller || !GruntAI)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Incorrect Controller or character being used with node 'GruntPlayerFoundChasing.cpp"));
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	// pull out our targeted player index
	int CurrentPlayerIndex = GruntAI->GetCurrentTargetPlayerIndex();
	// if its valid and the number is a valid index into our array just incase things dont match up
	if (CurrentPlayerIndex == -1)	// make sure we actually have a valid player
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No valid player to target for Grunt"));

		// since the target is invalid set back to patrolling
		GruntAI->SetAttackingState(AGruntAIEnemy::AttackStates::Patrolling);
		Controller->GetBlackboard()->SetValueAsBool(CanSeePlayer.SelectedKeyName, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
	
	// now that we know the index is valid get the target and use that to find the vector
	ACharacter* TargetPlayer = GruntAI->GetPlayerCharacters()[CurrentPlayerIndex];
	FVector VectorToPlayer = TargetPlayer->GetActorLocation() - GruntAI->GetActorLocation();
	
	// update the moving to check
	UpdateMovingStatus(GruntAI);
	
	// check attacking state with switch statement
	switch (GruntAI->GetCurrentAttackState())
	{
		case AGruntAIEnemy::Approaching:
			{
				if (GruntAI->IsGruntMoving)  // if we're already calling move to then we dont need to do this again
				{
					// get distances
					const float DistToKeep = GruntAI->GetDistanceToKeepFromPlayer();
					const float CurrentDistance = VectorToPlayer.Size();
					// if we are within range then switch to strafing
					if (CurrentDistance < DistToKeep)
					{
						GruntAI->IsGruntMoving = false;	// set to false that since strafing the current move to gets overrode
						GruntAI->SetAttackingState(AGruntAIEnemy::Strafing);
					}
					break;
				}
				// get our distances 
				const float DistToKeep = GruntAI->GetDistanceToKeepFromPlayer();
				const float CurrentDistance = VectorToPlayer.Size();

				// if we are within range then switch to strafing
				if (CurrentDistance < DistToKeep)
				{
					GruntAI->IsGruntMoving = false;	// set to false that since strafing the current move to gets overrode
					GruntAI->SetAttackingState(AGruntAIEnemy::Strafing);
				} else	// else do our approaching
				{	// get our needed variables for calculations
					const float VariationAngle = GruntAI->GetApproachVariationAngle();
					const float SharpnessAngle = GruntAI->GetApproachSharpnessAngle();
					const float DistanceBetweenStrafes = GruntAI->GetApproachDistance();

					// go ahead and normalize
					VectorToPlayer = VectorToPlayer.GetSafeNormal();
				
					// choose what direction to rotate
					const float Choice = FMath::RandRange(0.0f, 1.0f);
					if (Choice >= 0.5f)
					{
						VectorToPlayer = VectorToPlayer.RotateAngleAxis(SharpnessAngle, FVector(0, 0, 1));
					} else
					{
						VectorToPlayer = VectorToPlayer.RotateAngleAxis(-SharpnessAngle, FVector(0, 0, 1));
					}
					// after direction is chosen apply the variation angle
					VectorToPlayer = VectorToPlayer.RotateAngleAxis(FMath::FRandRange(-VariationAngle, VariationAngle), FVector(0, 0, 1));
					// now that final direction is chosen we set the length
					const FVector TargetPosition = GruntAI->GetActorLocation() + VectorToPlayer * DistanceBetweenStrafes;

					// we have final location but we need this to be a move to call with a valid
					UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
					if(!NavSys)	// check to see if the nav sys was found, if not return false
					{
						//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Nav Mesh Volume not found"));
						FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
					}
				
					FNavLocation Loc;
					if (NavSys->GetRandomPointInNavigableRadius(TargetPosition, 10, Loc, nullptr))
					{
						if (GruntAI->ShowDebug)
							DrawDebugLine(GetWorld(), GruntAI->GetActorLocation(), Loc.Location,
								FColor::Blue, false, 0.5f, 0, 5);
						// must set the max walk speed to use our desired movement speed
						GruntAI->GetCharacterMovement()->MaxWalkSpeed = GruntAI->GetMovementSpeed();
						// Call Move to on that point
						Controller->MoveToLocation(Loc.Location, GruntAI->GetMoveToTolerance());
						// set the grunt so that we are moving
						GruntAI->IsGruntMoving = true;
						// also set the destination location
						GruntAI->SetCurrentAttackingDestination(Loc.Location);
					}
				}
			}
			break;
		case AGruntAIEnemy::Strafing:
			{
				if (GruntAI->IsGruntMoving)  // if we're already calling move to then we dont need to do this again
				{
					break;
				}
				const float DistToKeep = GruntAI->GetDistanceToKeepFromPlayer();
				const float CurrentDistance = VectorToPlayer.Size();

				// if the player is too far away then start approaching again
				if (CurrentDistance > (DistToKeep+1000))
				{
					GruntAI->SetAttackingState(AGruntAIEnemy::Approaching);
					GruntAI->IsGruntMoving = false;
					break;
				}
				// get my vars needed for strafing around
				const float SharpnessAngle = GruntAI->GetStrafingSharpnessAngle();
				const float VariationAngle = GruntAI->GetStrafingVariationAngle();
				const float StrafeDistance = GruntAI->GetStrafingDistance();

				// normalize
				VectorToPlayer.Normalize();
				//rotate vector to either right or left
				const float Choice = FMath::FRandRange(0.0, 1.0);
				if (Choice >= 0.5f)
				{
					VectorToPlayer = VectorToPlayer.RotateAngleAxis(SharpnessAngle, FVector::UpVector);
				} else
				{
					VectorToPlayer = VectorToPlayer.RotateAngleAxis(-SharpnessAngle, FVector::UpVector);
				}
				// rotate by the variation
				VectorToPlayer = VectorToPlayer.RotateAngleAxis(FMath::FRandRange(-VariationAngle, VariationAngle), FVector::UpVector);
				// multiply by our distance
				VectorToPlayer *= StrafeDistance;
				//calculate our final location
				FVector ResultingLocation = GruntAI->GetActorLocation() + VectorToPlayer;
				
				//start of ray
				FVector TraceStart = GruntAI->GetActorLocation();
				//ignore player that fired it just in case
				const AActor* ActorToIgnore = GruntAI;
				//set up trace parameters
				FCollisionQueryParams TraceParams(FName(TEXT("RayTracing")), true, ActorToIgnore);
				TraceParams.bReturnPhysicalMaterial = false;
				TraceParams.bTraceComplex = true;

				FHitResult HitResult;
	
				//ray trace check for first collision 
				bool HadHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, ResultingLocation, ECC_Visibility, TraceParams);
				
				if (HadHit)
				{
					// get new location to move to since we would have been running into a wall
					FVector VectorToHitLocation = HitResult.Location - GruntAI->GetActorLocation();
					float HitDistance = (VectorToHitLocation).Size();
					// multiply by .85 so we only go 85% of the way to the object hit just so we dont get too close
					ResultingLocation = ResultingLocation.GetSafeNormal() * HitDistance * 0.85;
				}
				
				// we have final location but we need this to be a move to call with a valid
				UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
				if(!NavSys)	// check to see if the nav sys was found, if not return false
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Nav Mesh Volume not found"));
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				}
				
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(ResultingLocation, 5, Loc, nullptr))
				{
					if (GruntAI->ShowDebug)
						DrawDebugLine(GetWorld(), GruntAI->GetActorLocation(), Loc.Location,
							FColor::Red, false, 1.5f, 0, 5);
					// Call Move to on that point
					Controller->MoveToLocation(Loc.Location, GruntAI->GetMoveToTolerance());
					// set the grunt so that we are moving
					GruntAI->IsGruntMoving = true;
					// also set the destination location
					GruntAI->SetCurrentAttackingDestination(Loc.Location);
					// set that we no longer need a new attack destination
					GruntAI->SetNeedNewAttackDestination(false);
				}
			}
			break;
		case AGruntAIEnemy::Shooting:
			{
				// pretty much all shooting will be handled in Enemies/Grunt/GruntAIEnemy.cpp Tick method
				// this is so that we can get a better more consistent delta time each frame to decrease timers
				const FRotator Rotator = VectorToPlayer.Rotation();
				GruntAI->SetActorRotation(Rotator);
			}
			break;
		default:
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Attack state is not correct for grunt"));
			break;
	}
}

void UBTTask_GruntPlayerFoundChasing::FindNewTarget(TArray<ACharacter*> Players, AGruntAIEnemy* GruntAI)
{
	if (!GruntAI->HasPlayerToAttack())	// if we dont have any players set then we fail immediately 
	{
		GruntAI->SetCurrentTargetPlayerIndex(-1);	// set target to null since it doesnt have anything
		return;
	}
	
	ACharacter* TargetPlayer = Players[0];	// since we may have only one player so far just set the target to that player

	// get the location of our grunt and both players and calc. the distance to both of them
	const FVector Player1Location = Players[0]->GetActorLocation();
	const FVector GruntLocation = GruntAI->GetActorLocation();
	const float DistanceToPlayer1 = (Player1Location - GruntLocation).Size();
	// pre set the target to player 0 since we know for sure we have one player
	GruntAI->SetCurrentTargetPlayerIndex(0);
	if (Players.Num() > 1)
	{
		// we have two players so get the location and distance
		const FVector Player2Location = Players[1]->GetActorLocation();
		const float DistanceToPlayer2 = (Player2Location - GruntLocation).Size();
		if (DistanceToPlayer1 > DistanceToPlayer2)
		{
			// player one is closest so thats the target
			GruntAI->SetCurrentTargetPlayerIndex(1);
			TargetPlayer = Players[0];
		}
	}
	
}

void UBTTask_GruntPlayerFoundChasing::UpdateMovingStatus(AGruntAIEnemy* GruntAI)
{
	const FVector Destination = GruntAI->GetCurrentAttackingDestination();
	const float DistanceToDestination = (GruntAI->GetActorLocation() - Destination).Size();
	if (DistanceToDestination <= GruntAI->GetMoveToTolerance()+80 )
	{
		// if grunt is within range of the target destination then he will no longer be moving
		// and we can give him another point
		GruntAI->IsGruntMoving = false;
		// if we reach out point and we are in strafing then we can shoot
		// if we need a new attack destination then we cant go back to shooting
		if (GruntAI->GetCurrentAttackState() == AGruntAIEnemy::Strafing && !GruntAI->GetNeedNewAttackDestination())
		{
			GruntAI->SetAttackingState(AGruntAIEnemy::Shooting);
			if (GruntAI->ShowDebug)
			{
				//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Shooting player"));
			}
		}
	}
}