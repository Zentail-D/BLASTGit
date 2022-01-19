// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DronePatrolling.h"

#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"


UBTTask_DronePatrolling::UBTTask_DronePatrolling(FObjectInitializer const& object_initializer)
{
	bNotifyTick=true;
	NodeName=TEXT("DronePatrolling");
}

EBTNodeResult::Type UBTTask_DronePatrolling::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	return EBTNodeResult::Type::InProgress;
}

void UBTTask_DronePatrolling::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EnemyController = Cast<AEnemyControllerParent>(OwnerComp.GetAIOwner());
	if(EnemyController)
	{
		EngineerDrone = Cast<AEngineerDrones>(EnemyController->GetPawn());
		if(EngineerDrone)
		{
			if(EngineerDrone->CheckDestinationLocation())
			{
				//Checks to see the location is reachable
				if(EngineerDrone->GetPatrollingPath())
				{
					for (int Index = 0; Index < EngineerDrone->GetPatrollingPath()->NumOfPoint();Index++)
					{
						FVector FinalLocation = EngineerDrone->GetPatrollingPath()->GetPatrolPoint(EngineerDrone->GetPatrollingPathIndex());
						FinalLocation = EngineerDrone->GetPatrollingPath()->GetActorTransform().TransformPosition(FinalLocation);
						TArray<AActor*> IgnoreActors;
						IgnoreActors.Push(EngineerDrone);
						FHitResult Outhit;
						if(!UKismetSystemLibrary::SphereTraceSingle(GetWorld(),EngineerDrone->GetActorLocation(),
							FinalLocation,30,ETraceTypeQuery::TraceTypeQuery1,false,
							IgnoreActors,EDrawDebugTrace::None,Outhit,true))
						{
							EngineerDrone->SetDestinationLocation(FinalLocation);
							Index=EngineerDrone->GetPatrollingPath()->NumOfPoint() +1;
						}
						else
						{
							EngineerDrone->IncrementPatrollingPathIndex();
						}
					}
				}
				else
				{
					EngineerDrone->bNeedDestroyed=true;
					FinishLatentTask(OwnerComp,EBTNodeResult::Type::Failed);
				}
			}
		}
	}
	if (EnemyController == nullptr || EngineerDrone == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	InitialFinalDistance = DistanceToFinal();
	//Checking to see if we are within the tolerance of end distance
	if(DistanceToFinal()<= EngineerDrone->GetMoveToTolerance())
	{
		if(EngineerDrone->GetPatrollingPath())
		{
			EngineerDrone->IncrementPatrollingPathIndex();
		}
		else
		{
			EngineerDrone->bNeedDestroyed=true;
		}
		EngineerDrone->EmptyDestinationLocation();
		FinishLatentTask(OwnerComp,EBTNodeResult::Type::Succeeded);
	}
	//Are we going too fast and need to go slower
	if(BelowMaxVelocity())
	{
		FVector ForceToAdd = ForceVector() * EngineerDrone->GetAcceleration();
		//Checking to see if we are trying to do a force vector that is too large it was a error in calculation
		if((EngineerDrone->GetActorLocation() +ForceToAdd).Size()>20000)
		{
		}
		else
		{
			EngineerDrone->GetCapsuleComponent()->AddForce(ForceToAdd, TEXT("None"),true);
		}
	}
	//RotateToFaceMovingDirection();
}

float UBTTask_DronePatrolling::DistanceToFinal() const
{
	return (EngineerDrone->GetActorLocation()-EngineerDrone->GetDestinationLocation()).Size();
}

FVector UBTTask_DronePatrolling::ForceVector() const
{
	//Lerp the force from direction we want to go to the 0 vector so that the closer we are to the point the smaller force we apply
	FVector UnitVectorDirection = (EngineerDrone->GetDestinationLocation() - EngineerDrone->GetActorLocation()).GetSafeNormal();
	float const AlphaValue = (InitialFinalDistance - DistanceToFinal()) / InitialFinalDistance;
	UnitVectorDirection=UnitVectorDirection + AlphaValue * (FVector(0,0,0) - UnitVectorDirection);
	return UnitVectorDirection;
}

bool UBTTask_DronePatrolling::BelowMaxVelocity() const
{
	UCapsuleComponent* DroneCapsuleComp = EngineerDrone->GetCapsuleComponent();
	float const VectorLength = DroneCapsuleComp->GetPhysicsLinearVelocity(TEXT("None")).Size();
	return VectorLength <= EngineerDrone->GetMovementSpeed();
}

void UBTTask_DronePatrolling::RotateToFaceMovingDirection() const
{
	FRotator const EndResultRotation = FRotationMatrix::MakeFromX(EngineerDrone->GetDestinationLocation()-EngineerDrone->GetActorLocation()).Rotator();
	FQuat const FirstFQuat(EngineerDrone->GetActorRotation());
	FQuat  const SecondFQuat(EndResultRotation);
	FRotator const StepInRotation = FQuat::Slerp(FirstFQuat, SecondFQuat, EngineerDrone->GetRotationAlpha()).Rotator();
	EngineerDrone->SetActorRotation(StepInRotation);
}
