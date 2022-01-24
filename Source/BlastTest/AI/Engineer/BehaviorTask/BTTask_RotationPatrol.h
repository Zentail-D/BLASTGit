// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RotationPatrol.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ERotation : uint8 {
	VE_Left      UMETA(DisplayName="Left"),
	VE_Right       UMETA(DisplayName="Right"),
	VE_Neutral        UMETA(DisplayName="Neutral"),
};
UCLASS()
class  UBTTask_RotationPatrol : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	/**
	* Sets the Node Name
	*/
	UBTTask_RotationPatrol(FObjectInitializer const& object_initializer);
	/**
	* Set the roation that we want to move to using the Rotate To Face BB Entry built in task
	* @param OwnerComp Get Reference to Current Enemy and Controller
	* @param NodeMemory Needed to override the Function
	*/
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

private:
	/**Enum to tell you where the rotation should end up at. Neutral means starting location, left mean rotating clockwise and right is counterclockwise*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	ERotation RotationEnum;

	/*Blackboard key to the Rotation the turret is going towards. Created in the Execute task*/
	UPROPERTY(EditAnywhere, Category = Blackboard, meta = (AllowPrivateAccess = "true"))
	struct FBlackboardKeySelector RotationTurret;
};
