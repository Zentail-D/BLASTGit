// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RotationPatrol.h"

#include "EnemyControllerParent.h"
#include "EngineerTurret.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_RotationPatrol::UBTTask_RotationPatrol(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("RotationPatrol");
}

EBTNodeResult::Type UBTTask_RotationPatrol::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AEnemyControllerParent* const ContParent = Cast<AEnemyControllerParent>(owner_comp.GetAIOwner());
	if(ContParent)
	{
		AEngineerTurret* EngineerTurret = Cast<AEngineerTurret>(ContParent->GetPawn());
		if(EngineerTurret)
		{
			EngineerTurret->GetCharacterMovement()->RotationRate.Yaw = EngineerTurret->GetTurretPatrolRotationRate();
			//IF you set a enum in execute it will overide what you initalized it at
			switch (RotationEnum)
			{
			case ERotation::VE_Left:
				{
					ContParent->GetBlackboard()->SetValueAsRotator(RotationTurret.SelectedKeyName,EngineerTurret->GetStartingRotation()-FRotator(0,EngineerTurret->GetDegreeRotation(),0));
					RotationEnum=ERotation::VE_Right;
					break;
				}
			case ERotation::VE_Neutral:
				{
					ContParent->GetBlackboard()->SetValueAsRotator(RotationTurret.SelectedKeyName,EngineerTurret->GetStartingRotation());
					RotationEnum=ERotation::VE_Left;
					break;
				}
			case ERotation::VE_Right:
				{
					ContParent->GetBlackboard()->SetValueAsRotator(RotationTurret.SelectedKeyName,EngineerTurret->GetStartingRotation()+FRotator(0,EngineerTurret->GetDegreeRotation(),0));
					RotationEnum=ERotation::VE_Left;
					break;
				}
			default:
				{
					ContParent->GetBlackboard()->SetValueAsRotator(RotationTurret.SelectedKeyName,EngineerTurret->GetStartingRotation());
					RotationEnum=ERotation::VE_Neutral;
					break;
				}
			}
			FinishLatentTask(owner_comp,EBTNodeResult::Type::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	FinishLatentTask(owner_comp,EBTNodeResult::Type::Failed);
	return EBTNodeResult::Failed;
}
