// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindPatrolPath.generated.h"

/**
 * 
 */
UCLASS()
class  UFindPatrolPath : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	// Constructor to set up the node name and anything else
	UFindPatrolPath(FObjectInitializer const& object_initializer);
	
	/** main task function that gets ran from BT */
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	/** FVector to the location wanted*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector TargetLocation;

	/** Int that represents the current index were at for the patrol path */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector CurPatrolPathIndex;

	/** Bool that says if we need a new path */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector NeedNewPatrolPath;

	/** Bool that says if we need a new path */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector NeedNewAttackPath;

	/**Object types that need to be checked for collision*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	TArray<TEnumAsByte<EObjectTypeQuery>> object_types;

	/**Class of actors to filer out in the path finding*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	UClass* actorclassfilter;
};
