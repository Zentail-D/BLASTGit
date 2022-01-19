// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/EnemyControllerParent.h"
#include "FlyingEnemyAI.h"
#include "FlyingEnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class BLAST_API AFlyingEnemyAIController : public AEnemyControllerParent
{
	GENERATED_BODY()

public:
	AFlyingEnemyAIController();

	TArray<FVector> PathToTarget;

	int PathToTargetIndex;

protected:
	virtual void OnPossess(APawn* const pawn) override;

	virtual void Tick(float DeltaSeconds) override;

	
};
