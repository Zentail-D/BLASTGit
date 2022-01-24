// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GruntFindPatrolPathPoint.generated.h"

/**
 * 
 */
UCLASS()
class  UBTTask_GruntFindPatrolPathPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:	
	// Constructor to set up the node name and anything else
	UBTTask_GruntFindPatrolPathPoint(FObjectInitializer const& object_initializer);
	
	/** main task function that gets ran from BT */
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

protected:	
	/** FVector to the location wanted*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector TargetLocation;

	/** Int that represents the current index were at for the patrol path */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector CurPatrolPathIndex;

	/** Int that represents the current index were at for the patrol path */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector NeedNewPatrolPoint;

};
