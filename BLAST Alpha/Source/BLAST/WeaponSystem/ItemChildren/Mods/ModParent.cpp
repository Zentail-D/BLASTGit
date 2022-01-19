// Fill out your copyright notice in the Description page of Project Settings.


#include "ModParent.h"

AModParent::AModParent()
{
	if(!MeshComponent)
	{	
		MeshComponent= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
		MeshComponent->bHiddenInGame=true;
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}

void AModParent::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
}

void AModParent::ActiveModRelease(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
}

void AModParent::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

FString AModParent::GetModTypeName()
{
	return FString("Mod Does Not Contain An Override for GetModTypeName");
}
