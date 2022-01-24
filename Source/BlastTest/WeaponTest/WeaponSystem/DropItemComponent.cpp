// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/DropItemComponent.h"
#include "WeaponTest/WeaponSystem/ItemChildren/Mods/ModParent.h"

// Sets default values for this component's properties
UDropItemComponent::UDropItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

UDropItemComponent::~UDropItemComponent()
{
	ModOptions.Empty();
}

void UDropItemComponent::DropRandomMod(const FVector SpawnLocation)
{
	if(ModOptions.Num()>0)
	{
		const int Index =FMath::RandRange(1,ModOptions.Num());
		const FActorSpawnParameters SpawnParams;
		AModParent* ModParent = GetWorld()->SpawnActor<AModParent>(ModOptions[Index-1],SpawnLocation, FRotator(0,0,0), SpawnParams);
		if(!ModParent)
		{
			GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red, TEXT("Mod did not spawn in"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,TEXT("There was not a mod set inside of the option so It doesnt know what to drop do better"));
	}
}



