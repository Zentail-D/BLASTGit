// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIEnemyParent.h"
#include "EngineerToolsParent.generated.h"

UCLASS()
class  AEngineerToolsParent : public AAIEnemyParent
{
	GENERATED_BODY()

public:
	/**
	* Enable the tick for the Engineer Tools
	*/
	AEngineerToolsParent();

	/**
	 * Overriding function to call when a tool needs to be destroyed
	 */
	virtual void SafeToolDestroy();

	/**
	 * Sets the Engineer Enemy Parent to the Param
	 * @param EngineerEnemy Reference to Engineer Enemy
	 */
	void SetEngineerParent(class AEngineerEnemy*EngineerEnemy);

	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Tool Settings", meta = (AllowProtectedAccess = "true"))
	AEngineerEnemy* EngineerEnemyParent;
	
};
