// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/Flying/FlyingEnemyAIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AFlyingEnemyAIController::AFlyingEnemyAIController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AFlyingEnemyAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	GetBlackboard()->SetValueAsBool(TEXT("NeedNewPatrolPath"), true);
}

void AFlyingEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
