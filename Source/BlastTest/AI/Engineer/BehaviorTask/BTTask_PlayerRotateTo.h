// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PlayerRotateTo.generated.h"

/**
 * 
 */
UCLASS()
class  UBTTask_PlayerRotateTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	/**
	* Sets the Node Name
	*/
	UBTTask_PlayerRotateTo(FObjectInitializer const& object_initializer);
	/**
	* Finds the player to rotate to and set it to the blackboard key
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	*/
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	/**Blackboard key to set the character to rotate towards*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector TargetCharacter;

	/**Blackboard key to set the character index to rotate towards*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector TargetCharacterIndex;
};
