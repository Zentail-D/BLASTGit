// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SpawnEngineerTool.h"
#include "EngineerEnemy.h"
#include "EngineerMine.h"
#include "EngineerTurret.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UBTTask_SpawnEngineerTool::UBTTask_SpawnEngineerTool(FObjectInitializer const& object_initializer)
{
	bNotifyTick=true;
	NodeName = TEXT("Spawn Engineer Tool");
}

EBTNodeResult::Type UBTTask_SpawnEngineerTool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		Engineer = Cast<AEngineerEnemy>(EnemyController->GetPawn());
		if(Engineer)
		{
			EnemyController->GetBlackboard()->SetValueAsFloat(FiringCooldown.SelectedKeyName,Engineer->GetAttackingCooldown());
			EnemyController->GetBlackboard()->SetValueAsFloat(SpawningTimer.SelectedKeyName,Engineer->GetSpawningTimer());
			Engineer->EmptyDestinationLocation();
		}
		return EBTNodeResult::Type::InProgress;
	}
	return EBTNodeResult::Type::Failed;
}

void UBTTask_SpawnEngineerTool::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		Engineer = Cast<AEngineerEnemy>(EnemyController->GetPawn());
		if(Engineer)
		{
			//Only start the spawning movement when the timer reaches zero
			float SpawningStartTimer = EnemyController->GetBlackboard()->GetValueAsFloat(SpawningTimer.SelectedKeyName);
			SpawningStartTimer-=DeltaSeconds;
			if(SpawningStartTimer<=0)
			{
				//Spawn in tool when cooldown reaches zero
				float SpawnCooldown = EnemyController->GetBlackboard()->GetValueAsFloat(FiringCooldown.SelectedKeyName);
				SpawnCooldown-=DeltaSeconds;
				if(SpawnCooldown<=0)
				{
					EnemyController->StopMovement();
					SpawnRandomTool();
					Engineer->EmptyDestinationLocation();
					FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
				}
				EnemyController->GetBlackboard()->SetValueAsFloat(FiringCooldown.SelectedKeyName,SpawnCooldown);
				//Move Around Randomly until we spawn in a tool
				if(Engineer->CheckDestinationLocation())
				{
					FVector const FinalLocation = Engineer->GetActorLocation();
					Engineer->GetCharacterMovement()->MaxWalkSpeed=Engineer->GetMovementSpeed();
					float const X = FMath::FRandRange(-1,1);
					float const Y = FMath::FRandRange(-1,1);
					FVector const DesiredLocation = FinalLocation + FVector(X,Y,0) * OffSetFromPathingToSpawn;
					UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
					if(!NavSys)
					{
						if(GEngine)
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Nav Mesh Volume not found"));
						}
						FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
					}
					FNavLocation Loc;
					if (NavSys->GetRandomPointInNavigableRadius(DesiredLocation, Engineer->GetPatrolPointRadiusTolerance(), Loc, nullptr))
					{
						EnemyController->MoveToLocation(Loc.Location,-1);
						Engineer->SetDestinationLocation(Loc.Location);
					}
				}
			
				if (EnemyController == nullptr || Engineer == nullptr)
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				}
				//Checking to see if we are within the tolerance of end distance
				if(DistanceToFinal()<= Engineer->GetMoveToTolerance())
				{
					Engineer->EmptyDestinationLocation();
				}
			}
			EnemyController->GetBlackboard()->SetValueAsFloat(SpawningTimer.SelectedKeyName,SpawningStartTimer);
		}
	}
}

bool UBTTask_SpawnEngineerTool::CheckCollision(FVector DesiredLocation) const
{
	TArray<AActor*> OutActors;
	return UKismetSystemLibrary::BoxOverlapActors(GetWorld(),DesiredLocation,FVector(75)
		,ObjectTypes,ActorFilterClass,TArray<AActor*>(),OutActors);
}

void UBTTask_SpawnEngineerTool::SpawnRandomTool() const
{
	//Get a spawn location
	FVector SpawningLocation = Engineer->GetActorLocation() + Engineer->GetActorForwardVector()*250;
	SpawningLocation.Z += 100;	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Engineer;
	SpawnParams.Instigator = Engineer->GetInstigator();
		
	if(!CheckCollision(SpawningLocation))
	{
		if(Engineer->GetPlayerCharacters().Num()!=0)
		{
			int const RandomNum = FMath::RandRange(1,Engineer->GetEngineerTools().Num());
			AEngineerToolsParent* EngineerTool = GetWorld()->SpawnActor<AEngineerToolsParent>(Engineer->GetEngineerTools()[RandomNum-1], SpawningLocation, Engineer->GetActorRotation(), SpawnParams);
			if(Cast<AEngineerMine>(EngineerTool))
			{
				if(!Engineer->CanSpawnInMine())
				{
					EngineerTool->Destroy();
				}
				else if(EngineerTool)
				{
					EngineerTool->SetPlayerCharacters(Engineer->GetPlayerCharacters());
					EngineerTool->SetEngineerParent(Engineer);
					//Engineer->AddEngineerToolToArray(EngineerTool);
					Engineer->AddOneToCurrentNumberOfMines();
				}
			}
			else if(Cast<AEngineerTurret>(EngineerTool))
			{
				if(!Engineer->CanSpawnInTurret())
				{
					EngineerTool->Destroy();
				}
				else if(EngineerTool)
				{
					EngineerTool->SetPlayerCharacters(Engineer->GetPlayerCharacters());
					EngineerTool->SetEngineerParent(Engineer);
					Engineer->AddEngineerToolToArray(EngineerTool);
					Engineer->AddOneToCurrentNumberOfTurret();
				
				}
			}
		
		}
	}
}

float UBTTask_SpawnEngineerTool::DistanceToFinal() const
{
	return (Engineer->GetActorLocation()-Engineer->GetDestinationLocation()).Size();
}
