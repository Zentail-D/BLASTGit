// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FlyingEnemyAI.h"
#include "FlyingEnemyAIController.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "DrawDebugHelpers.h"
#include "AdvanceTowardsAndAttackPlayer.generated.h"

/**
 * 
 */
UCLASS()
class  UAdvanceTowardsAndAttackPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:	
	// Constructor to set up the node name and anything else
	UAdvanceTowardsAndAttackPlayer(FObjectInitializer const& object_initializer);

	/** with ticktask in here this will get called once every time the node is visited
	*  like a kind of constructor */
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

	/** what will get called every run through of the behavior tree */
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	/** Will be set when end of navigation path is reached and we need a new navigaton path */
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector NeedNewAttackPath;
	
private:

	/** this boolean is what we use to tell if we are at the end of the current navigation path */
	bool IsAtEndOfPath = false;

	/** applies a force to the ai in the direction of the AI
	* @param TargetLocation - The point at which the force will be applied towards
	*/
	void ApplyForceToAI(FVector TargetLocation, AFlyingEnemyAI* FlyingAI, FColor color);

	/** Used for determening if the player has reached the current target in the navigation path
	* @param CurrentTargetLocation - the location the function will test against
	* @return bool - true if within some tolerance of target, false if not
	*/
	bool CurrentTargetReached(FVector CurrentTargetLocation, AFlyingEnemyAI* FlyingAI);

	/** turn physics off fo rthe capsule component */
	static void TurnOffPhysics(const AFlyingEnemyAI* FlyingAI);

	/** turns the physics back on for the capsule component end turns gravity off*/
	static void TurnOnPhysics(const AFlyingEnemyAI* FlyingAI);
};
