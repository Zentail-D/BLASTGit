// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SpawnDrone.h"
#include "EngineerDrones.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_SpawnDrone::UBTTask_SpawnDrone(FObjectInitializer const& object_initializer)
{
	NodeName=TEXT("Spawn Drone");
	bNotifyTick=true;
}

EBTNodeResult::Type UBTTask_SpawnDrone::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		Engineer = Cast<AEngineerEnemy>(EnemyController->GetPawn());
		if(Engineer)
		{
			//If we want a instant spawn it will set the spawning timer to 0 else to the drone spawning delay timer
			if(bInstantSpawn)
			{
				EnemyController->GetBlackboard()->SetValueAsFloat(SpawningTimer.SelectedKeyName,0);
			}
			else
			{
				if(EnemyController->GetBlackboard()->GetValueAsBool(CanSeeTarget.SelectedKeyName))
				{
					EnemyController->GetBlackboard()->SetValueAsFloat(SpawningTimer.SelectedKeyName,Engineer->GetDroneSpawningDelayTimerPlayer());
					//GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Red,TEXT("Player stuff"));
				}
				else
				{
					EnemyController->GetBlackboard()->SetValueAsFloat(SpawningTimer.SelectedKeyName,Engineer->GetDroneSpawningDelayTimerPatrolling());
					//GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Red,TEXT("Patrolling stuff"));
				}
				//Engineer->EmptyDestinationLocatiton();
			}
		}
		return EBTNodeResult::Type::InProgress;
	}
	return EBTNodeResult::Type::Failed;
}

void UBTTask_SpawnDrone::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		Engineer = Cast<AEngineerEnemy>(EnemyController->GetPawn());
		if(Engineer)
		{
			//When timer runs out call spawn drones function
			float DroneSpawnTimer = EnemyController->GetBlackboard()->GetValueAsFloat(SpawningTimer.SelectedKeyName);
			DroneSpawnTimer-=DeltaSeconds;
			if(DroneSpawnTimer<=0)
			{
				SpawnDrones(NumberOfDroneToSpawn);
				FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
			}
			EnemyController->GetBlackboard()->SetValueAsFloat(SpawningTimer.SelectedKeyName,DroneSpawnTimer);
		}
	}
}

bool UBTTask_SpawnDrone::CheckCollision(FVector DesiredLocation) const
{
	TArray<AActor*> OutActors;
	return UKismetSystemLibrary::BoxOverlapActors(GetWorld(),DesiredLocation,FVector(75)
		,ObjectTypes,ActorFilterClass,TArray<AActor*>(),OutActors);
}

void UBTTask_SpawnDrone::SpawnDrones(int NumOfDrones) const
{
	//Loop for each drone desired
	for(int Index=0; Index <NumOfDrones;Index++)
	{
		//Get a spawn location
		FVector SpawningLocation = RandomLocationAroundEngineer();
		SpawningLocation.Z += 100;	
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Engineer;
		SpawnParams.Instigator = Engineer->GetInstigator();
		//Check if we didnt hit anything
		if(!CheckCollision(SpawningLocation))
		{
			if(Engineer->CanSpawnInDrone())
			{
				if(Engineer->GetPlayerCharacters().Num()!=0)
				{
					//Spawn the actual actor of blueprint class
					AEngineerToolsParent* EngineerToolsParent = GetWorld()->SpawnActor<AEngineerToolsParent>(
						Engineer->GetDroneClass(), SpawningLocation, Engineer->GetActorRotation(), SpawnParams);
			
					if(EngineerToolsParent)
					{
						AEngineerDrones* Drones = Cast<AEngineerDrones>(EngineerToolsParent);
						if(Drones)
						{
							//Setting the values that need to be initialized
							Drones->SetPlayerCharacters(Engineer->GetPlayerCharacters());
							Drones->SetPatrollingPath(Engineer->GetRandomDronePath());
							Drones->SetDroneSwarm(Engineer->GetEngineerDroneSwarm());
							Drones->SetEngineerParent(Engineer);
							//Engineer->AddEngineerToolToArray(Drones);
							Engineer->AddOneToCurrentNumberOfDrones();
						}
					}
				}
			}
		}
	}
}

FVector UBTTask_SpawnDrone::RandomLocationAroundEngineer() const
{
	float const X = FMath::FRandRange(-1,1);
	float const Y = FMath::FRandRange(-1,1);
	FVector const RandomLocation = FVector(X,Y,0);
	return Engineer->GetActorLocation() + RandomLocation*RandomDistanceFromEngineer;
}

