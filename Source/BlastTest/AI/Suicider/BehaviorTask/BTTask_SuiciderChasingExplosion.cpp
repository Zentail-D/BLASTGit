// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SuiciderChasingExplosion.h"
#include "NetworkChar.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_SuiciderChasingExplosion::UBTTask_SuiciderChasingExplosion(FObjectInitializer const& object_initializer)
{
	bNotifyTick=true;
	NodeName =TEXT("Suicider Chasing Explosion");
}

EBTNodeResult::Type UBTTask_SuiciderChasingExplosion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		SuiciderEnemy = Cast<ASuiciderEnemy>(EnemyController->GetPawn());
		if(SuiciderEnemy)
		{
			int const PlayerIndex = GetClosestPlayer();
			EnemyController->GetBlackboard()->SetValueAsInt(TargetCharacterIndex.SelectedKeyName,PlayerIndex);
			if(PlayerIndex>=0)
			{
				//Empty the Destination so that we find a new when we enter this task and save off the firing cooldown to the blackboard
				SuiciderEnemy->GetCharacterMovement()->MaxWalkSpeed=SuiciderEnemy->GetMovementSpeed();
				EnemyController->GetBlackboard()->SetValueAsFloat(AttackingCooldown.SelectedKeyName,SuiciderEnemy->GetAttackingCooldown());
				EnemyController->MoveToActor(SuiciderEnemy->GetPlayerCharacters()[PlayerIndex]);
				return EBTNodeResult::Type::InProgress;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Red,TEXT("There was not a player character set in the array make sure to set one"));
				FinishLatentTask(OwnerComp,EBTNodeResult::Type::Failed);
				return EBTNodeResult::Type::Failed;
			}
		}
	}
	FinishLatentTask(OwnerComp,EBTNodeResult::Type::Failed);
	return EBTNodeResult::Type::Failed;
}

void UBTTask_SuiciderChasingExplosion::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		SuiciderEnemy = Cast<ASuiciderEnemy>(EnemyController->GetPawn());
		if(SuiciderEnemy)
		{
			//Checking to see if we are within the tolerance of end distance
			if(DistanceToFinal()<= SuiciderEnemy->GetAttackingRange())
			{
				EnemyController->StopMovement();
				bStartChargeUpAttack=true;
			}
			if(bStartChargeUpAttack)
			{
				float ExplosionTime = EnemyController->GetBlackboard()->GetValueAsFloat(AttackingCooldown.SelectedKeyName);
				ExplosionTime-=DeltaSeconds;
				if(ExplosionTime<=0)
				{
					SuiciderAttackingBehavior();
					FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
				}
				//GEngine->AddOnScreenDebugMessage(-1,0.01,FColor::Red,FVector(ExplosionTime).ToString());
				EnemyController->GetBlackboard()->SetValueAsFloat(AttackingCooldown.SelectedKeyName,ExplosionTime);
				if(ExplosionTime>=InstantDeathTime)
				{
					if(DistanceToFinal()>= SuiciderEnemy->GetExplosionRadius())
					{
						EnemyController->GetBlackboard()->SetValueAsFloat(AttackingCooldown.SelectedKeyName,SuiciderEnemy->GetAttackingCooldown());
						bStartChargeUpAttack=false;
						int const PlayerIndex = GetClosestPlayer();
						if(PlayerIndex>=0)
						{
							EnemyController->GetBlackboard()->SetValueAsInt(TargetCharacterIndex.SelectedKeyName,PlayerIndex);
							EnemyController->GetBlackboard()->SetValueAsFloat(AttackingCooldown.SelectedKeyName,SuiciderEnemy->GetAttackingCooldown());
							EnemyController->MoveToActor(SuiciderEnemy->GetPlayerCharacters()[PlayerIndex]);
						}
						else
						{
							if(SuiciderEnemy->GetPlayerCharacters().Num()!=0)
							{
								EnemyController->MoveToActor(SuiciderEnemy->GetPlayerCharacters()[EnemyController->GetBlackboard()->GetValueAsInt(TargetCharacterIndex.SelectedKeyName)]);
							}
						}
						//GEngine->AddOnScreenDebugMessage(-1,0.01,FColor::Red,FVector(ExplosionTime).ToString());
					}
				}
			}
		}
	}
}

float UBTTask_SuiciderChasingExplosion::DistanceToFinal() const
{
	if(SuiciderEnemy->GetPlayerCharacters().Num() != 0)
	{
		return (SuiciderEnemy->GetActorLocation()-SuiciderEnemy->GetPlayerCharacters()[EnemyController->GetBlackboard()->GetValueAsInt(TargetCharacterIndex.SelectedKeyName)]->GetActorLocation()).Size();
	}
	return -1;
}

void UBTTask_SuiciderChasingExplosion::SuiciderAttackingBehavior() const
{
	switch (SuiciderEnemy->GetSuiciderAttackingState())
	{
		case ESuiciderAttackEnums::VE_None:
			{
				//GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Green,TEXT("The Suicider Has the Attack Pattern of None (Still Needs Implemented Defaults to Red)"));
				SuiciderExplosion();
				break;
			}
		case ESuiciderAttackEnums::VE_Green:
			{
				//GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Green,TEXT("The Suicider Has the Attack Pattern of Green (Still Needs Implemented Defaults to Red)"));
				SuiciderExplosion();
				break;
			}
		case ESuiciderAttackEnums::VE_Red:
			{
				//GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Green,TEXT("The Suicider Has the Attack Pattern of Red"));
				SuiciderExplosion();
				break;
			}
		case ESuiciderAttackEnums::VE_Blue:
			{
				//GEngine->AddOnScreenDebugMessage(-1,1.5,FColor::Green,TEXT("The Suicider Has the Attack Pattern of Blue (Still Needs Implemented Defaults to Red)"));
				SuiciderExplosion();
				break;
			}
		default:
			{
				break;
			}
	}
}

int UBTTask_SuiciderChasingExplosion::GetClosestPlayer() const
{
	TArray<ACharacter*> Players = SuiciderEnemy->GetPlayerCharacters();
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
				float const Player1Distance =(Player1Location-SuiciderEnemy->GetActorLocation()).Size();
				float const Player2Distance =(Player2Location-SuiciderEnemy->GetActorLocation()).Size();

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

void UBTTask_SuiciderChasingExplosion::SuiciderExplosion() const
{
	TArray<ACharacter*> Players = SuiciderEnemy->GetPlayerCharacters();
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
				float const Player1Distance =(Player1Location-SuiciderEnemy->GetActorLocation()).Size();
				float const Player2Distance =(Player2Location-SuiciderEnemy->GetActorLocation()).Size();

				if(SuiciderEnemy->GetExplosionRadius() > Player1Distance)
				{
					ANetworkChar* PlayerCharacter =Cast<ANetworkChar>(Player1);
					if(PlayerCharacter)
					{
						PlayerCharacter->DealDamageToPlayer(SuiciderEnemy->GetDamageAmount());
					}
				}

				if(SuiciderEnemy->GetExplosionRadius() > Player2Distance)
				{
					ANetworkChar* PlayerCharacter =Cast<ANetworkChar>(Player2);
					if(PlayerCharacter)
					{
						PlayerCharacter->DealDamageToPlayer(SuiciderEnemy->GetDamageAmount());
					}
				}
			}
		}
		else
		{
			ACharacter* Player =Cast<ACharacter>(Players[0]);
			if(Player)
			{
				FVector const Player1Location = Player->GetActorLocation();
				float const Player1Distance =(Player1Location-SuiciderEnemy->GetActorLocation()).Size();
				 if(SuiciderEnemy->GetExplosionRadius() > Player1Distance)
				 {
				 	ANetworkChar* PlayerCharacter =Cast<ANetworkChar>(Player);
				 	if(PlayerCharacter)
				 	{
				 		PlayerCharacter->DealDamageToPlayer(SuiciderEnemy->GetDamageAmount());
				 	}
				 }
			}
		}
	}
	SuiciderEnemy->bNeedDestroyed=true;
}
