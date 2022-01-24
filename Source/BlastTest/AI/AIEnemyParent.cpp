// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyParent.h"

#include <Engine/World.h>

#include "EnemyProjectile.h"
#include "EnemyControllerParent.h"

// Sets default values
AAIEnemyParent::AAIEnemyParent()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Creating a MuzzleLocation
	UStaticMeshComponent* AMuzzleLocation= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleLocation"));
	AMuzzleLocation->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform);
	AMuzzleLocation->bHiddenInGame=true;
	AMuzzleLocation->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//Direct Path to the asset to make it so that the location is visible in the editor
	static ConstructorHelpers::FObjectFinder<UStaticMesh>TempMesh(TEXT("StaticMesh'/Game/BLAST/Gameplay/WepSys/Sphere.Sphere'"));
	if(TempMesh.Succeeded())
	{
		AMuzzleLocation->SetStaticMesh(TempMesh.Object);
		AMuzzleLocation->SetRelativeScale3D(FVector(0.09,0.09,0.09));

	}
	AddMuzzleLocation(AMuzzleLocation);

	// sets the attacking cooldown reset as a backup for us to reset attacking cooldown when needed
	AttackCooldownReset = AttackingCooldown;

	if(!NiagaraParticleSystem)
	{
		NiagaraParticleSystem= CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraParticleSystem"));
		NiagaraParticleSystem->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform);
	}

	if(!InvokerComponent)
	{
		InvokerComponent= CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("InvokerComponent"));
	}

	/*if(!DropItemComponent)
	{
		DropItemComponent= CreateDefaultSubobject<UDropItemComponent>(TEXT("DropItemComponent"));
	}*/
}

// Called when the game starts or when spawned
void AAIEnemyParent::BeginPlay()
{
	Super::BeginPlay();
	//Saving off the attack cooldown so that we can reset the timer
	AttackCooldownReset=AttackingCooldown;
}

void AAIEnemyParent::DestroyChildren()
{
}

void AAIEnemyParent::IncrementMuzzleIndex()
{
	MuzzleLocationIndex++;
	if(MuzzleLocationIndex>=MuzzleLocations.Num())
	{
		MuzzleLocationIndex=0;
	}
}

// Called every frame
void AAIEnemyParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// SWITCH THAT HANDLES ALL THE ENEMY STATUS EFFECTS (TALK TO NICK BEFORE EDITING)
	switch (CurrentEnemyStatusEffect)
	{
		case EWeaponStatusEffects::Ve_Fire:	// handles when the enemy has a fire status effect
			{
				if(!DecrementFireStatusEffectDuration(DeltaTime))	// continues if we are still on fire
				{
					if (DecrementFireStatusEffectDamageFrequency(DeltaTime))	// continues when the frequency reaches zero
					{
						DealDamageToEnemy(FireStatusEffectDamage); // deal our damage
						ResetFireStatusEffectDamageFrequencyTimer();	// resets the frequency timer
					}
				} else	// ran when the fire effect runs out
				{
					// make sure to reset timer once we are done burning
					ResetFireStatusEffectDuration();
					// NOTE: we do not set the frequency timer here since if we do that the fire effect will lose a tick since
					// it should damage right away when fire effect starts
					SetEnemyStatusEffect(EWeaponStatusEffects::Ve_None);	// once done status is over so set to none
				}
			}
		default:	// switch statement formality
			{
				break;
			}
	}
	if(CurrentHealth<=0)
	{
		//death
		if(HasAuthority())
		{
			/*if(DropItemComponent)
			{
				DropItemComponent->DropRandomMod(GetActorLocation());
			}*/
		}
		FreezeBehavior();
		DestroyChildren();
		Destroy();
		
	}

}

// Called to bind functionality to input
void AAIEnemyParent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

TArray<ACharacter*> AAIEnemyParent::GetPlayerCharacters()
{
	return PlayerCharacters;
}

APatrollingPath* AAIEnemyParent::GetPatrollingPath()
{
	return PatrollingPath;
}

void AAIEnemyParent::SetPatrollingPath(APatrollingPath* NewPatrollingPath)
{
	PatrollingPath= NewPatrollingPath;
}

void AAIEnemyParent::SetPlayerCharacters(TArray<ACharacter*> NewPlayerCharacters)
{
	PlayerCharacters=NewPlayerCharacters;
}

void AAIEnemyParent::Fire(int PlayerCharacterIndex)
{
	if (ProjectileClass)
	{
		//Check to see if the index is too big
		if(PlayerCharacterIndex<PlayerCharacters.Num())
		{


			//Get the selected player location and estimating the location that the player will be at by the time the bullet get to the player
			FVector TargetLocation= PlayerCharacters[PlayerCharacterIndex]->GetActorLocation();
			float DistanceToPlayer =GetDistanceTo(PlayerCharacters[PlayerCharacterIndex]);
			FVector TargetVelocity = PlayerCharacters[PlayerCharacterIndex]->GetVelocity();
			DistanceToPlayer = DistanceToPlayer / FMath::FRandRange(BulletSpeed - BulletSpeed/3,BulletSpeed+ BulletSpeed/3);
			FVector EstimatedTargetLocation =TargetVelocity * DistanceToPlayer;
			EstimatedTargetLocation = EstimatedTargetLocation+TargetLocation;

			//Finding the rotation need for the bullet so it fire in the correct direction
			FRotator Rotation = FRotationMatrix::MakeFromX(EstimatedTargetLocation-GetActorLocation()).Rotator();
			FRotator InRotation;
			FQuat AQuat(GetActorRotation());
			FQuat BQuat(Rotation);

			FQuat Result = FQuat::Slerp(AQuat, BQuat, 1.0);

			InRotation =Result.Rotator();
			
			
			
			


			FVector MuzzleLocation;
			if(MuzzleLocationIndex<MuzzleLocations.Num())
			{
				MuzzleLocation = MuzzleLocations[MuzzleLocationIndex]->GetComponentLocation();

			}
			else
			{
				MuzzleLocation = MuzzleLocations[0]->GetComponentLocation();

			}


			//Use the next muzzle offset if we have one for the next bullet
			IncrementMuzzleIndex();
			
			UWorld* World = GetWorld();
			if (World)
			{
				//Parameters for spawning in a actor
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();
				SpawnParams.bNoFail = true;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// Spawn the projectile at the muzzle.
				AEnemyProjectile* Projectile = World->SpawnActor<AEnemyProjectile>(ProjectileClass, MuzzleLocation, InRotation, SpawnParams);
				if (Projectile)
				{
					// Set the projectile's initial trajectory and damage
					FVector LaunchDirection = InRotation.Vector();
					Projectile->ProjectileMovementComponent->InitialSpeed=BulletSpeed;
					Projectile->FireInDirection(LaunchDirection);
					Projectile->SetDamageAmount(DamageAmount);
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,TEXT("Failed to spawn in"));
				}
			}
		}
		else
		{
			if(GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,TEXT("PlayerCharacterIndex is out of Range"));
			}
		}
	}
	else
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1,1.5f,FColor::Red,TEXT("Attempted to Fire Without a Projectile Class Set"));
		}
	}
}

bool AAIEnemyParent::FreezeBehavior()
{
	//If we are not frozen then we freeze
	if(!bFrozen)
	{
		AEnemyControllerParent* ControllerParent = Cast<AEnemyControllerParent>(GetController());
		if(ControllerParent)
		{
			ControllerParent->FreezeBehavior();
			bFrozen=true;
			return true;
		}
	}
	return false;
}

bool AAIEnemyParent::UnFreezeBehavior()
{
	//If we are frozen then we unfreeze
	if(bFrozen)
	{
		AEnemyControllerParent* ControllerParent = Cast<AEnemyControllerParent>(GetController());
		if(ControllerParent)
		{
			ControllerParent->UnFreezeBehavior();
			bFrozen=false;
			return true;
		}
	}
	return false;
}

void AAIEnemyParent::DealDamageToEnemy(int Damage)
{
	CurrentHealth=CurrentHealth-Damage;
	
}

void AAIEnemyParent::DecreaseAttackingCooldown(float TimeToSubtract)
{
	AttackingCooldown -= TimeToSubtract;
}

int AAIEnemyParent::GetDamageAmount()
{
	return DamageAmount;
}

void AAIEnemyParent::AddMuzzleLocation(UStaticMeshComponent* NewMuzzleLocation)
{
	MuzzleLocations.Add(NewMuzzleLocation);
}

float AAIEnemyParent::GetAttackingCooldown()
{
	return AttackingCooldown;
}

void AAIEnemyParent::ResetAttackingCooldown()
{
	AttackingCooldown = AttackCooldownReset;
}

float AAIEnemyParent::GetMovementSpeed()
{
	return MovementSpeed;
}

float AAIEnemyParent::GetCurrentHealth()
{
	return CurrentHealth;
}

float AAIEnemyParent::GetTotalHealth()
{
	return TotalHealth;
}

int AAIEnemyParent::GetScoreAmount()
{
	return ScoreAmount;
}

int AAIEnemyParent::GetMoveToTolerance()
{
	return MoveToTolerance;
}

int AAIEnemyParent::GetPlayerFoundTolerance()
{
	return PlayerFoundTolerance;
}

float AAIEnemyParent::GetAttackingRange()
{
	return AttackingRange;
}

bool AAIEnemyParent::DecrementFireStatusEffectDuration(float DeltaTime)
{
	CurrentFireStatusEffectDuration -= DeltaTime;
	if (CurrentFireStatusEffectDuration <= 0.f)
		return true;
	return false;
}

bool AAIEnemyParent::DecrementFireStatusEffectDamageFrequency(float DeltaTime)
{
	CurrentFireStatusEffectDamageFrequencyTimer -= DeltaTime;
	if (CurrentFireStatusEffectDamageFrequencyTimer <= 0.f)
		return true;
	return false;
}

void AAIEnemyParent::SetEnemyStatusEffect( const EWeaponStatusEffects Status)
{
	// switch to handle the logic of status effect application
	switch (CurrentEnemyStatusEffect)
	{
	case EWeaponStatusEffects::Ve_Fire:	// we already have fire status effect applied so just reset the duration if it wants to be re-applied
		{
			ResetFireStatusEffectDuration();
			break;
		}
	case EWeaponStatusEffects::Ve_None:	// if we dont have a status effect apply the desired effect
		{
			CurrentEnemyStatusEffect = Status;
			break;
		}
	default:	// just a formality for a switch statement
		{
			break;
		}
	}
}

