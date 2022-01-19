// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/ShotgunProjectile.h"

#include "DrawDebugHelpers.h"

AShotgunProjectile::AShotgunProjectile()
{
    CollisionComponent->SetBoxExtent(GetSpread());
}

// Called every frame
void AShotgunProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Spread.Y+=GrowSpeed;
	//Spread.Z+=GrowSpeed;
	//CollisionComponent->SetBoxExtent(Spread);
	DrawDebugBox(GetWorld(),GetActorLocation(),CollisionComponent->GetScaledBoxExtent(),FColor::Green,false,0.1,0,3);

}

void AShotgunProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
void AShotgunProjectile::SetSpread(FVector NewSpread)
{
	Spread = NewSpread;
}

FVector AShotgunProjectile::GetSpread() const
{
	return Spread;

}void AShotgunProjectile::SetGrowSpeed(float NewGrowSpeed)
 {
 	GrowSpeed = NewGrowSpeed;
 }
 
 float AShotgunProjectile::GetGrowSpeed() const
 {
 	return GrowSpeed;
 }