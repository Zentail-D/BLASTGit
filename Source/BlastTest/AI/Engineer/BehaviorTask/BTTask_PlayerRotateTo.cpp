// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PlayerRotateTo.h"

#include "AIEnemyParent.h"
#include "EnemyControllerParent.h"
#include "EngineerTurret.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_PlayerRotateTo::UBTTask_PlayerRotateTo(FObjectInitializer const& object_initializer)
{
	NodeName= TEXT("Player Rotate To");
}

EBTNodeResult::Type UBTTask_PlayerRotateTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemyControllerParent* const EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		AAIEnemyParent* EnemyParent = Cast<AAIEnemyParent>(EnemyController->GetPawn());
		if(EnemyParent)
		{
			if(Cast<AEngineerTurret>(EnemyParent))
			{
				Cast<AEngineerTurret>(EnemyParent)->GetCharacterMovement()->RotationRate.Yaw=Cast<AEngineerTurret>(EnemyParent)->GetTurretPlayerFoundRotationRate();
			}
			//Checking which player is closeset
			TArray<ACharacter*> Players = EnemyParent->GetPlayerCharacters();
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
						float const Player1Distance =(Player1Location-EnemyParent->GetActorLocation()).Size();
						float const Player2Distance =(Player2Location-EnemyParent->GetActorLocation()).Size();

						if(Player1Distance>Player2Distance)
						{
							//GEngine->AddOnScreenDebugMessage(-1,0.5f,FColor::Orange,TEXT("Picked Player 2"));
							EnemyController->GetBlackboard()->SetValueAsObject(TargetCharacter.SelectedKeyName,Player2);
							EnemyController->GetBlackboard()->SetValueAsInt(TargetCharacterIndex.SelectedKeyName,1);
						}
						else
						{
							//GEngine->AddOnScreenDebugMessage(-1,0.5f,FColor::Orange,TEXT("Picked Player 1"));
							EnemyController->GetBlackboard()->SetValueAsObject(TargetCharacter.SelectedKeyName,Player1);
							EnemyController->GetBlackboard()->SetValueAsInt(TargetCharacterIndex.SelectedKeyName,0);

						}
						FinishLatentTask(OwnerComp,EBTNodeResult::Type::Succeeded);
						return EBTNodeResult::Type::Succeeded;
					}
					else
					{
						if(GEngine)
						{
							GEngine->AddOnScreenDebugMessage(-1,0.5f,FColor::Orange,TEXT("Failed to cast the player1 and player 2 to a character"));
						}
					}
				}
				else
				{
					ACharacter* Player =Cast<ACharacter>(Players[0]);
					if(Player)
					{
						EnemyController->GetBlackboard()->SetValueAsObject(TargetCharacter.SelectedKeyName,Player);
						EnemyController->GetBlackboard()->SetValueAsInt(TargetCharacterIndex.SelectedKeyName,0);
						FinishLatentTask(OwnerComp,EBTNodeResult::Type::Succeeded);
						return EBTNodeResult::Type::Succeeded;
					}
					else
					{
						if(GEngine)
						{
							
						
							GEngine->AddOnScreenDebugMessage(-1,0.5f,FColor::Orange,TEXT("Only Character in array failed to cast to a character"));
						}
					}
				}
			}
			else
			{
				if(GEngine)
				{
					
				
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,TEXT("No Player was set"));
				}
			}
		}
	}
	FinishLatentTask(OwnerComp,EBTNodeResult::Type::Failed);
	return EBTNodeResult::Type::Failed;
}
