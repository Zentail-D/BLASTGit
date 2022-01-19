// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MineExplosion.h"
#include "NetworkChar.h"
#include "AIEnemyParent.h"
#include "EnemyControllerParent.h"
#include "EngineerMine.h"
#include "Perception/AISenseConfig_Sight.h"

UBTTask_MineExplosion::UBTTask_MineExplosion(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Mine Explosion");
}

EBTNodeResult::Type UBTTask_MineExplosion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get TargetLocation from blackboard via AI controller
	AEnemyControllerParent* const EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		AAIEnemyParent* EnemyParent = Cast<AAIEnemyParent>(EnemyController->GetPawn());
		if(EnemyParent)
		{
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

						if(EnemyController->SenseConfig_Sight->SightRadius > Player1Distance)
						{
							
								ANetworkChar* PlayerChar= Cast<ANetworkChar>(Player1);
								if(PlayerChar)
								{
									PlayerChar->DealDamageToPlayer(EnemyParent->GetDamageAmount());
								}
								Cast<AEngineerMine>(EnemyParent)->bNeedDestroyed=true;
						}

						if(EnemyController->SenseConfig_Sight->SightRadius > Player2Distance)
						{
							ANetworkChar* PlayerChar= Cast<ANetworkChar>(Player2);
							if(PlayerChar)
							{
								PlayerChar->DealDamageToPlayer(EnemyParent->GetDamageAmount());
							}
							Cast<AEngineerMine>(EnemyParent)->bNeedDestroyed=true;
						}
					}
				}
				else
				{
					ACharacter* Player =Cast<ACharacter>(Players[0]);
					if(Player)
					{
						ANetworkChar* PlayerChar= Cast<ANetworkChar>(Player);
						if(PlayerChar)
						{
							PlayerChar->DealDamageToPlayer(EnemyParent->GetDamageAmount());
						}
						Cast<AEngineerMine>(EnemyParent)->bNeedDestroyed=true;
					}
				}
			}
			else
			{
				if(GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Red,TEXT("Player was not set"));
				}
			}
		}
	}
	FinishLatentTask(OwnerComp,EBTNodeResult::Type::Succeeded);
	return EBTNodeResult::Type::Succeeded;
	
}
