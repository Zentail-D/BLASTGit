// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GruntAIEnemy.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GruntPlayerFoundChasing.generated.h"

/**
 * 
 */
UCLASS()
class  UBTTask_GruntPlayerFoundChasing : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// Constructor to set up the node name and anything else
	UBTTask_GruntPlayerFoundChasing(FObjectInitializer const& object_initializer);
	
	/** main task function that gets ran from BT */
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	/** what will get called every run through of the behavior tree */
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	/** Int that represents the current index were at for the patrol path */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector NeedNewPatrolPoint;
	
	/** FVector to the location wanted*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector CanSeePlayer;

	static void FindNewTarget(TArray<ACharacter*> Players, AGruntAIEnemy* GruntAI);

	void UpdateMovingStatus(AGruntAIEnemy* GruntAI);
	
};
