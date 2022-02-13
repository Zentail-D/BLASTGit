// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Grunt/BehaviorTask/BTTask_GruntPlayerFoundChasing.h"

#include "DrawDebugHelpers.h"
#include "EnemyControllerParent.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

class AGruntAIEnemy;
// Constructor to set up the node name and anything else
UBTTask_GruntPlayerFoundChasing::UBTTask_GruntPlayerFoundChasing(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Grunts Player Found Chasing");
	bNotifyTick = true;
}
	
/** main task function that gets ran from BT */
EBTNodeResult::Type UBTTask_GruntPlayerFoundChasing::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// initialize the controller and character
	AEnemyControllerParent* const Controller = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	AGruntAIEnemy* GruntAI = Cast<AGruntAIEnemy>(Controller->GetPawn());
	if (!Controller || !GruntAI)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Type::Failed;
	}
	
	// Right when we get to the node set a target for when we approach
	GruntAI->SetCurrentTargetPlayerIndex(GetClosestPlayer(GruntAI));
	// also set the need new patrol point if the player gets out of sight of enemy
	Controller->GetBlackboard()->SetValueAsBool(NeedNewPatrolPoint.SelectedKeyName, true);
	if(MoveToLocation(GruntAI,Controller))
	{
		return  EBTNodeResult::Type::InProgress;
	}
	return EBTNodeResult::Type::Failed;
} 

/** what will get called every run through of the behavior tree */
void UBTTask_GruntPlayerFoundChasing::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// initialize the controller and character
	AEnemyControllerParent* const Controller = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(Controller)
	{
		AGruntAIEnemy* GruntAI = Cast<AGruntAIEnemy>(Controller->GetPawn());
		if(GruntAI)
		{
			if(Controller->GetPathFollowingComponent()->DidMoveReachGoal())
			{
				if(GruntAI->bReadyToFire)
				{
					GruntAI->bFireBurst =true;
					GruntAI->bReadyToFire=false;
				}
				if(GruntAI->bFireBurst)
				{
					// pretty much all shooting will be handled in Enemies/Grunt/GruntAIEnemy.cpp Tick method
					// this is so that we can get a better more consistent delta time each frame to decrease timers
					ACharacter* TargetPlayer = GruntAI->GetPlayerCharacters()[GruntAI->GetCurrentTargetPlayerIndex()];
					const FVector VectorToPlayer = TargetPlayer->GetActorLocation() - GruntAI->GetActorLocation();
					const FRotator Rotator = VectorToPlayer.Rotation();
					GruntAI->SetActorRotation(Rotator);
				}
				else
				{
					// we have final location but we need this to be a move to call with a valid
					UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
					if(NavSys)
					{
						FNavLocation Loc;
						if (NavSys->GetRandomPointInNavigableRadius(GruntAI->GetCurrentStrafingLocation(), GruntAI->GetMoveToTolerance(), Loc, nullptr))
						{
							if (GruntAI->ShowDebug)
								DrawDebugLine(GetWorld(), GruntAI->GetActorLocation(), Loc.Location,
									FColor::Blue, false, 0.5f, 0, 5);
							// must set the max walk speed to use our desired movement speed
							GruntAI->GetCharacterMovement()->MaxWalkSpeed = GruntAI->GetMovementSpeed();
							// Call Move to on that point
							Controller->MoveToLocation(Loc.Location, GruntAI->GetMoveToTolerance());
							// set the grunt so that we are moving
							// also set the destination location
							GruntAI->SetCurrentAttackingDestination(Loc.Location);
							if(GruntAI->IncrementStrafingPathIndex())
							{
								FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
							}
							else
								GruntAI->bQueuedFire=true;
							
						}
					}
				}
			}
		}
	}
}



int UBTTask_GruntPlayerFoundChasing::GetClosestPlayer(AGruntAIEnemy* GruntAI) const
{
	TArray<ACharacter*> Players = GruntAI->GetPlayerCharacters();
	//Checking to see if there is a player in the array or at least one if there is one we just check that one if there is two we check both
	if(Players.Num()>0)
	{
		if(Players.Num() >1)
		{
			ACharacter* Player1 = Cast<ACharacter>(Players[0]);
			ACharacter* Player2 = Cast<ACharacter>(Players[1]);

			if(Player1 && Player2)
			{
				FVector const Player1Location = Player1->GetActorLocation();
				FVector const Player2Location = Player2->GetActorLocation();
				float const Player1Distance =(Player1Location-GruntAI->GetActorLocation()).Size();
				float const Player2Distance =(Player2Location-GruntAI->GetActorLocation()).Size();

				if(Player1Distance>Player2Distance)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}
		else
		{
			return 0;
		}
	}
	return -1;
}

bool UBTTask_GruntPlayerFoundChasing::MoveToLocation(AGruntAIEnemy* GruntAI, AEnemyControllerParent* EnemyController) const
{
	if(GruntAI)
	{
		if(EnemyController)
		{
			const int CurrentPlayerIndex = GruntAI->GetCurrentTargetPlayerIndex();

			// pull out our targeted player index
			// if its valid and the number is a valid index into our array just in case things dont match up
			if (CurrentPlayerIndex == -1)	// make sure we actually have a valid player
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No valid player to target for Grunt"));

				// since the target is invalid set back to patrolling
				EnemyController->GetBlackboard()->SetValueAsBool(CanSeePlayer.SelectedKeyName, false);
				return false;
			}
			// now that we know the index is valid get the target and use that to find the vector
			const ACharacter* TargetPlayer = GruntAI->GetPlayerCharacters()[CurrentPlayerIndex];
			FVector VectorToPlayer =  TargetPlayer->GetActorLocation() - GruntAI->GetActorLocation();
			
			// get our needed variables for calculations
			const float VariationAngle = GruntAI->GetStrafingVariationAngle();
			const float SharpnessAngle = GruntAI->GetStrafingSharpnessAngle();
			const float DistanceBetweenStrafes = GruntAI->GetStrafingDistance();

			VectorToPlayer =VectorToPlayer.GetSafeNormal();

			
			

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
			const FVector TargetPosition = VectorToPlayer * DistanceBetweenStrafes + FVector(0,0,GruntAI->GetActorLocation().Z);
			CreatePath(GruntAI, TargetPosition);

			// we have final location but we need this to be a move to call with a valid
			UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
			if(NavSys)
			{
				FNavLocation Loc;
				if (NavSys->GetRandomPointInNavigableRadius(GruntAI->GetCurrentStrafingLocation(), -1, Loc, nullptr))
				{
					if (GruntAI->ShowDebug)
						DrawDebugLine(GetWorld(), GruntAI->GetActorLocation(), Loc.Location,
							FColor::Blue, false, 0.5f, 0, 5);
					// must set the max walk speed to use our desired movement speed
					GruntAI->GetCharacterMovement()->MaxWalkSpeed = GruntAI->GetMovementSpeed();
					// Call Move to on that point
					EnemyController->MoveToLocation(Loc.Location, GruntAI->GetMoveToTolerance());
					// set the grunt so that we are moving
					// also set the destination location
					GruntAI->SetCurrentAttackingDestination(Loc.Location);
					GruntAI->IncrementStrafingPathIndex();
					GruntAI->bQueuedFire=true;
					return true;
				}
			}
		}
	}
	return false;
	
}

void UBTTask_GruntPlayerFoundChasing::CreatePath(AGruntAIEnemy* GruntAI, const FVector EndLocation)
{
	const int GruntSubSection = GruntAI->GetStrafingSubsection();
	const float PercentageSubSection = 1 / static_cast<float>(GruntSubSection);

	const FVector VectorToLocation =  EndLocation - GruntAI->GetActorLocation();
	TArray<FVector> Path;
	for(int Index = 1;Index <= GruntSubSection;Index++)
	{
		FVector TempLocation = VectorToLocation*(PercentageSubSection*Index) +GruntAI->GetActorLocation();
		Path.Add(TempLocation);
	}
	GruntAI->SetStrafingPath(Path);
}
