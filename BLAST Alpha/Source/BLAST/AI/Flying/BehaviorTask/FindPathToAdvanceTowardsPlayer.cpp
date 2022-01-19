// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Flying/BehaviorTask/FindPathToAdvanceTowardsPlayer.h"

#include "DrawDebugHelpers.h"
#include "NetworkChar.h"
#include "BehaviorTree/BlackboardComponent.h"

UFindPathToAdvanceTowardsPlayer::UFindPathToAdvanceTowardsPlayer()
{
	NodeName = TEXT("Find Path To Advance Towards Player");
}

EBTNodeResult::Type UFindPathToAdvanceTowardsPlayer::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// find the controller and flying
	AFlyingEnemyAIController* Controller = Cast<AFlyingEnemyAIController>(owner_comp.GetAIOwner());
	AFlyingEnemyAI* FlyingAI = Cast<AFlyingEnemyAI>(Controller->GetPawn());
	if (Controller && FlyingAI)	// only proceed if controller and flyingAI was actually found
	{
		// first things first lets make sure to set the attacking state of the enemy
		FlyingAI->CurrentAttackState = AFlyingEnemyAI::Agro;

		// set our dampenings since we are in attacking
		FlyingAI->SetAttackingDampenings();
		
		// declair variables for using the NavVolume 3D
		FVector start = FlyingAI->GetActorLocation();
		FVector end = GetRandomPointAroundPlayer(FlyingAI);;
		TArray<FVector> path;
		if (FlyingAI->NavVolume3D && FlyingAI->NavVolume3D->FindPath(start, end, object_types, actorclassfilter, path))
		{
			// if find path returns true then we set the neccessary variables in the controller
			Controller->PathToTarget = path;
			Controller->PathToTargetIndex = 0;
		} else
		{
			Controller->PathToTargetIndex = -1;
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Could not find path in FindPathToPlayer.cpp or nav volume for flyer not set"));
		}
			
		// we found a new attack path so be sure to set that we do not need a new one
		Controller->GetBlackboard()->SetValueAsBool(NeedNewAttackPath.SelectedKeyName, false);
		// since we overwrote any patrolling path the con. may have had we set this to true so that if
		// the AI goes back to patrolling it knows to get a new path
		Controller->GetBlackboard()->SetValueAsBool(NeedNewPatrolPath.SelectedKeyName, true);

		// we will return out of the task now that everything is done
		FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
		
	} else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Count not find flying ai or controller in FindPathToPlayer.cpp"));
	}
	// still have to return something even if we fail
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}


FVector UFindPathToAdvanceTowardsPlayer::GetRandomPointAroundPlayer(AFlyingEnemyAI* MyEnemy)
{
	// lines 45-63 is to find what player is the closes
	FVector EnemyLocation = MyEnemy->GetActorLocation();
	TArray<ACharacter*> players = MyEnemy->GetPlayerCharacters();
	float DistToPlayer1 = MAX_FLT;
	float DistToPlayer2 = MAX_FLT;
	ACharacter* TargetPlayer;
	if (players.Num() > 0)	// we have atleast one player in array
	{
		if (players[0])
		{
			FVector p1location = players[0]->GetActorLocation();
			DistToPlayer1 = (p1location - EnemyLocation).Size();
		} else
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("player array index 0 does not have a valid player (FindPathToAdvanceTowardsPlayer.cpp"));
			return FVector(0, 0, 0);	// we need to return something so that the game isnt crashing
		}
		if (players.Num() > 1)	// we have more than one player in array
		{
			if (players[1])
			{
				FVector p2location = players[1]->GetActorLocation();
				DistToPlayer2 = (p2location - EnemyLocation).Size();
			} else
			{
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("player array index 1 does not have a valid player (FindPathToAdvanceTowardsPlayer.cpp"));
				return FVector(0, 0, 0);	// we need to return something so that the game isnt crashing
			}
		}
		// checks to see which player is closest and sets that target
		if (DistToPlayer1 <= DistToPlayer2)
		{
			TargetPlayer = players[0];
			MyEnemy->TargetedPlayerIndex = 0;
		}
		else
		{
			TargetPlayer = players[1];
			MyEnemy->TargetedPlayerIndex = 1;
		}
		
	} else	// we dont have any players in our list
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("No players found in Enemies Character TArray"));
		MyEnemy->TargetedPlayerIndex = -1;
		return FVector(0, 0, 0);
	}
	
	
	// get the direction from player to enemy
	FVector PlayerToEnemyVec = EnemyLocation - TargetPlayer->GetActorLocation();
	// get just the x and y components of the player to enemy vec and normalize it
	FVector HorizontalPlaneDirectionUnitVec = FVector(PlayerToEnemyVec.X, PlayerToEnemyVec.Y, 0).GetSafeNormal();
	// get both perpendicular unit vectors from the one pointing to the enemy from the player
	FVector PerpVec1 = FVector(-HorizontalPlaneDirectionUnitVec.Y, HorizontalPlaneDirectionUnitVec.X, 0).GetSafeNormal();
	FVector PerpVec2 = FVector(HorizontalPlaneDirectionUnitVec.Y, -HorizontalPlaneDirectionUnitVec.X, 0).GetSafeNormal();
	// flip a coin to see which direction the player will go
	FVector DirectionToSend;
	bool CoinFlip = FMath::RandBool();
	if (CoinFlip)	// we send the enemy in the direction of perpvec1
	{
		DirectionToSend = PerpVec1;
	} else		// we send the enemy in the direction of the perpvec2
	{
		DirectionToSend = PerpVec2;
	}
	//Calculate our target point with the direction and the player location and a random value between min and max height
	FVector TargetPoint = TargetPlayer->GetActorLocation() + (DirectionToSend * MyEnemy->GetDistanceToKeepFromPlayer());
	TargetPoint = TargetPoint + FVector(0, 0, FMath::RandRange(MyEnemy->GetMinHeightAboveGroundWhenAttacking(),
		MyEnemy->GetMaxHeightAboveGroundWhenAttacking()));
	
	if (MyEnemy->ShowDebug)
	{
		FVector pl = TargetPlayer->GetActorLocation();

		DrawDebugLine(GetWorld(), pl, pl + DirectionToSend*150, FColor::Green, false, 3.f, 0, 5);
		DrawDebugLine(GetWorld(), pl, pl + PerpVec2*150, FColor::Orange, false, 3.f, 0, 5);

	}
	// lets do a line trace strait down a given amount to make sure that we dont get too low to the ground
	FHitResult OutHit;
	FVector start = TargetPoint;
	FVector end = FVector(0, 0, -MyEnemy->GetMinHeightAboveGroundWhenAttacking()) + start;
	// must have this to do a line trace
	FCollisionQueryParams CollisionParams;
	//show helpful debug line of line trace
	if (MyEnemy->ShowDebug)
		DrawDebugLine(GetWorld(), start, end, FColor::Blue, false, 3.f, 0, 5);

	// this actually does the line trace
	if(GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_Visibility, CollisionParams))
	{
		if(OutHit.bBlockingHit)	// we have a hit
			{
			if (MyEnemy->ShowDebug)
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Enemy Too Low Adjusting Height"));
			// since we're too low to the ground set the point to the min height over the ground
			TargetPoint = OutHit.Location + FVector(0, 0, MyEnemy->GetMinHeightAboveGroundWhenAttacking());
			}
	}
	
	return TargetPoint;
}