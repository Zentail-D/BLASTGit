// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SpawningWave.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SpawningWave::UBTTask_SpawningWave(FObjectInitializer const& object_initializer)
{
	bNotifyTick=true;
	NodeName=TEXT("Spawning Wave");
}

EBTNodeResult::Type UBTTask_SpawningWave::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		EngineerSpawner = Cast<AEngineerSpawner>(EnemyController->GetPawn());
		if(EngineerSpawner)
		{
			EnemyController->GetBlackboard()->SetValueAsFloat(FiringCooldown.SelectedKeyName,EngineerSpawner->GetTimeTillNextSpawn());
		}
		return EBTNodeResult::Type::InProgress;
	}
	return EBTNodeResult::Type::Failed;
}

void UBTTask_SpawningWave::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		EngineerSpawner = Cast<AEngineerSpawner>(EnemyController->GetPawn());
		if(EngineerSpawner)
		{
			if(EngineerSpawner->GetIsActive())
			{
				//Calls the SpawnWave in the Spawner when timer runs out
				float SpawnCooldown = EnemyController->GetBlackboard()->GetValueAsFloat(FiringCooldown.SelectedKeyName);
				SpawnCooldown-=DeltaSeconds;
				if(SpawnCooldown<=0)
				{
					EngineerSpawner->SpawnWave();
					FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
				}
				EnemyController->GetBlackboard()->SetValueAsFloat(FiringCooldown.SelectedKeyName,SpawnCooldown);
			}
		}
	}
}
