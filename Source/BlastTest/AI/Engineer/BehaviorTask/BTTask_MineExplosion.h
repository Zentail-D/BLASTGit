// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MineExplosion.generated.h"

/**
 * 
 */
UCLASS()
class  UBTTask_MineExplosion : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	/**
	* Sets the Node Name
	*/
	UBTTask_MineExplosion(FObjectInitializer const& ObjectInitializer);
	/**
	* Always in progress
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	*/
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
