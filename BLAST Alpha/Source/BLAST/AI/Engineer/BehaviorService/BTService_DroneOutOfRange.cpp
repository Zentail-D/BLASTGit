// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DroneOutOfRange.h"

#include "EnemyControllerParent.h"
#include "EngineerDrones.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_DroneOutOfRange::UBTService_DroneOutOfRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Drone Out Of Range");
}

void UBTService_DroneOutOfRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	//Casting the Controller and Drone
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		EngineerDrone = Cast<AEngineerDrones>(EnemyController->GetPawn());
		if(EnemyController)
		{
			//Check to see if the Drone is out of range only set if False
			if(!FindPlayerInRange())
			{
				EnemyController->GetBlackboard()->SetValueAsBool(InRange.SelectedKeyName,false);

			}
		}
	}
}

FString UBTService_DroneOutOfRange::GetStaticServiceDescription() const
{
	return FString("Check if we are out of range");
}

bool UBTService_DroneOutOfRange::FindPlayerInRange()
{
	TArray<ACharacter*> Players = EngineerDrone->GetPlayerCharacters();
	//Check if there is a player else give Gengine print for error
	if(Players.Num()>0)
	{
		//Check if one or two player if one just check if one player is in range else check both
		if(Players.Num() >1)
		{
			ACharacter* Player1 = Cast<ACharacter>(Players[0]);
			ACharacter* Player2 = Cast<ACharacter>(Players[1]);
			//Gengine print if we are not checking characters
			if(Player1 && Player2)
			{
				FVector const Player1Location = Player1->GetActorLocation();
				FVector const Player2Location = Player2->GetActorLocation();
				float const Player1Distance =(Player1Location-EngineerDrone->GetActorLocation()).Size();
				float const Player2Distance =(Player2Location-EngineerDrone->GetActorLocation()).Size();
				//If the player distance is shorter then return true saying we are in range else return false
				if(Player1Distance<EngineerDrone->GetAttackingRange())
				{
					return true;
				}
				else if(Player2Distance<EngineerDrone->GetAttackingRange())
				{
					return true;
				}
				return false;
			}
			else
			{
				if(GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Purple,TEXT("One of the players failed to cast to Character"));
				}}
		}
		else
		{
			ACharacter* Player1 = Cast<ACharacter>(Players[0]);
			//Needs to be a character
			if(Player1)
			{
				//If we are in range return true else false
				FVector const Player1Location = Player1->GetActorLocation();
				float const Player1Distance =(Player1Location-EngineerDrone->GetActorLocation()).Size();
				if(Player1Distance<EngineerDrone->GetAttackingRange())
				{
					return true;
				}
				return false;
			}
			else
			{
				if(GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Purple,TEXT("Player in Array not Character"));
				}
			}
		}
	}
	else
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Purple,TEXT("There is not a player in the player array"));
		}
	}
	return false;
}
