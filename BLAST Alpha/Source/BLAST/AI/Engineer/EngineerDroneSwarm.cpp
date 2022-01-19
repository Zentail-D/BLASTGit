// Fill out your copyright notice in the Description page of Project Settings.


#include "EngineerDroneSwarm.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEngineerDroneSwarm::AEngineerDroneSwarm()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


TArray<AEngineerDrones*> AEngineerDroneSwarm::GetDroneSwarm() const
{
	return DroneSwarm;
}

void AEngineerDroneSwarm::AddNewDrone(AEngineerDrones* Drone)
{
	DroneSwarm.Add(Drone);
	Drone->SetDroneSwarm(this);
}

void AEngineerDroneSwarm::RemoveDroneAtIndex(int Index)
{
	if(Index > 0 && Index < DroneSwarm.Num())
	{
		DroneSwarm.RemoveAt(Index);
	}
	else
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red, TEXT("Drone Index out of Range"));
		}
	}
}

void AEngineerDroneSwarm::EmptyDrones()
{
	DroneSwarm.Empty();
}

FVector AEngineerDroneSwarm::PointOverlappingDrones(FVector Destination, FVector BoxExtent, TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, UClass* ActorClassFilter)
{
	//Checks to see if the location is overlapping with another drone
	TArray<AActor*> OutActors;
	TArray<int> CollidingIndex;
	for(int Index =0; Index < DroneSwarm.Num(); Index++)
	{
		if(UKismetSystemLibrary::BoxOverlapActors(GWorld, Destination, BoxExtent, ObjectTypes, ActorClassFilter, TArray<AActor*>(), OutActors))
		{
			CollidingIndex.Add(Index);
		}
	}
	//If no other drone is in that location then return the location
	if(CollidingIndex.Num()==0)
	{
		return Destination;
	}
	//If one drone is overlapping then take unit vector from that drone to the destination and multiply it by 2 times the box extent and return that location instead
	else if(CollidingIndex.Num()==1)
	{
		FVector const UnitVector =(DroneSwarm[CollidingIndex[0]]->GetActorLocation() - Destination).GetSafeNormal();
		FVector const NewDestination =UnitVector * BoxExtent.Size()*2;
		return NewDestination;
	}
	//If the one is overlapping two drone then do the same process as above but recursively try the new location until no overlap occurs
	else
	{
		FVector const UnitVector =(DroneSwarm[CollidingIndex[0]]->GetActorLocation() - Destination).GetSafeNormal();
		FVector const NewDestination =UnitVector * BoxExtent.Size()*2;
		return PointOverlappingDrones(NewDestination,BoxExtent,ObjectTypes,ActorClassFilter);
	}
}

