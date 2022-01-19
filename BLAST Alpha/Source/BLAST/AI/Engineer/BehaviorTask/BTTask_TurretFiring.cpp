// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurretFiring.h"

#include "EnemyControllerParent.h"
#include "EngineerTurret.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurretFiring::UBTTask_TurretFiring(FObjectInitializer const& object_initializer)
{
	bNotifyTick=true;
	NodeName = TEXT("Turret Firing");
}

EBTNodeResult::Type UBTTask_TurretFiring::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyControllerParent* const EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		AEngineerTurret* EnemyParent = Cast<AEngineerTurret>(EnemyController->GetPawn());
		if(EnemyParent)
		{
			EnemyController->GetBlackboard()->SetValueAsFloat(FiringCooldown.SelectedKeyName,EnemyParent->GetAttackingCooldown());
		}
		return EBTNodeResult::Type::InProgress;
	}
	FinishLatentTask(OwnerComp,EBTNodeResult::Type::Failed);
	return EBTNodeResult::Type::Failed;
}
void UBTTask_TurretFiring::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AEnemyControllerParent* const EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		AEngineerTurret* EnemyParent = Cast<AEngineerTurret>(EnemyController->GetPawn());
		if(EnemyParent)
		{
			//Calls the Fire function when the timer runs out
			float FireCooldown = EnemyController->GetBlackboard()->GetValueAsFloat(FiringCooldown.SelectedKeyName);
			FireCooldown-=DeltaSeconds;
			if(FireCooldown<=0)
			{
				EnemyParent->Fire(EnemyController->GetBlackboard()->GetValueAsInt(TargetCharacterIndex.SelectedKeyName));
				FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
			}
			EnemyController->GetBlackboard()->SetValueAsFloat(FiringCooldown.SelectedKeyName,FireCooldown);
		}
	}
}
