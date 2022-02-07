// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponTest/WeaponSystem/ShotgunProjectile.h"

AShotgunProjectile::AShotgunProjectile()
{
    
}

// Called every frame
void AShotgunProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AShotgunProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
}


