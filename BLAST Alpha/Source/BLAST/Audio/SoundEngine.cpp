// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/SoundEngine.h"
#include "SubtitleManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/AmbientSound.h"
#include "Sound/SoundClass.h"


// Sets default values
ASoundEngine::ASoundEngine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASoundEngine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called when the game ends or when removed
void ASoundEngine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ResetAudioLevels();
}

void ASoundEngine::ToggleSubtitles()
{
	bSubtitles = !bSubtitles;
	UGameplayStatics::SetSubtitlesEnabled(bSubtitles);
	//if(GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 50.0f, FColor::Red, bSubtitles ? "True" : "False");
	//}
}


#pragma region CalledFunctions

// Called to play an AI sound
void ASoundEngine::PlayAISound(TEnumAsByte<EType> AIName, TEnumAsByte<EAction> Action, FVector Location, UAudioComponent* AudioComp)
{
	if(AudioComp)
	{
		// All possible AI types
		switch(AIName)
		{
		case 0:
			// Flyer
			FlyerAction(Action, Location, AudioComp);
			break;
		
		case 1:
			// Suicider
			SuiciderAction(Action, Location, AudioComp);
			break;
		
		case 2:
			// Engineer
			EngineerAction(Action, Location, AudioComp);
			break;

		case 3:
			// Grunt
			GruntAction(Action, Location, AudioComp);
			break;
		
		default:
			// Not real AI type
			break;
		}
	}
	else
	{
		// All possible AI types
		switch(AIName)
		{
		case 0:
			// Flyer
			FlyerAction(Action, Location, nullptr);
			break;
		
		case 1:
			// Suicider
			SuiciderAction(Action, Location, nullptr);
			break;
		
		case 2:
			// Engineer
			EngineerAction(Action, Location, nullptr);
			break;

		case 3:
			// Grunt
			GruntAction(Action, Location, nullptr);
			break;
		
		default:
			// Not real AI type
			break;
		}
	}
}

// Called to play a weapon sound
void ASoundEngine::PlayWeaponSound(TEnumAsByte<EWeapon> WeaponName, FVector Location, UAudioComponent* AudioComp)  
{
	if(AudioComp)
	{
		// All weapon types
		switch(WeaponName)
		{
		case 0:
			// Default
			if(DefaultFire)
			{
				AudioComp->SetSound(DefaultFire);
			}
			AudioComp->FadeIn(0.5);
			//AudioComp->Play();
			break;
		case 1:
			// Shotgun
			if(ShotgunFire)
			{
				AudioComp->SetSound(ShotgunFire);
			}
			AudioComp->FadeIn(0.5);
			//AudioComp->Play();
			break;
		
		case 2:
			// RocketLauncher
			if(RocketLauncherFire)
			{
				AudioComp->SetSound(RocketLauncherFire);
			}
			AudioComp->FadeIn(0.5);
			//AudioComp->Play();
			break;
		
		case 3:
			// Beam
			if(BeamFire)
			{
				AudioComp->SetSound(BeamFire);
			}
			AudioComp->FadeIn(0.5);
			//AudioComp->Play();
			break;

		case 4:
			// FlameThrower
			if(FlameThrowerFire)
			{
				AudioComp->SetSound(FlameThrowerFire);
			}
			AudioComp->FadeIn(0.5);
			//AudioComp->Play();
			break;

		case 5:
			// Charge
			if(ChargeFire)
			{
				AudioComp->SetSound(ChargeFire);
			}
			AudioComp->FadeIn(0.5);
			//AudioComp->Play();
			break;
		
		default:
			// Not real weapon type
			break;
		}
	}
	else
	{
		// All weapon types
		switch(WeaponName)
		{
		case 0:
			// Default
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DefaultFire, Location);
			break;
		case 1:
			// Shotgun
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShotgunFire, Location);
			break;
		
		case 2:
			// RocketLauncher
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), RocketLauncherFire, Location);
			break;
		
		case 3:
			// Beam
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BeamFire, Location);
			break;

		case 4:
			// FlameThrower
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FlameThrowerFire, Location);
			break;

		case 5:
			// Charge
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ChargeFire, Location);
			break;
		
		default:
			// Not real weapon type
			break;
		}
	}
}

// Called to play a pickup drop sound
void ASoundEngine::PlayPickupDropSound(TEnumAsByte<EPickup> Pickup, FVector Location, UAudioComponent* AudioComp)
{
	if(AudioComp)
	{
		// All pickups
		switch(Pickup)
		{
		case 0:
			// Weapon
			if(WeaponDropped)
				AudioComp->SetSound(WeaponDropped);
			AudioComp->FadeIn(0.5);
			//AudioComp->Play();
			break;
		case 1:
			// Part
			if(PartDropped)
				AudioComp->SetSound(PartDropped);
			AudioComp->FadeIn(0.5);
			//AudioComp->Play();
			break;
		default:
			// Not real pickup
			break;
		}
	}
	else
	{
		// All pickups
		switch(Pickup)
		{
		case 0:
			// Weapon
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponDropped, Location);
			break;
		case 1:
			// Part
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PartDropped, Location);
			break;
		default:
			// Not real pickup
			break;
		}
	}
}

// Called to play a pickup pickup sound
void ASoundEngine::PlayPickupPickupSound(TEnumAsByte<EPickup> Pickup, FVector Location, UAudioComponent* AudioComp)
{
	if(AudioComp)
	{
		// All pickups
		switch(Pickup)
		{
		case 0:
			// Weapon
			if(WeaponPickup)
				AudioComp->SetSound(WeaponPickup);
			AudioComp->FadeIn(0.5);
			//AudioComp->Play();
			break;
		case 1:
			// Part
			if(PartPickup)
				AudioComp->SetSound(PartPickup);
			AudioComp->FadeIn(0.5);
			//AudioComp->Play();
			break;
		default:
			// Not real pickup
			break;
		}
	}
	else
	{
		// All pickups
		switch(Pickup)
		{
		case 0:
			// Weapon
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), WeaponPickup, Location);
			break;
		case 1:
			// Part
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), PartPickup, Location);
			break;
		default:
			// Not real pickup
			break;
		}
	}
}

// Called to stop the audio component from playing
void ASoundEngine::StopSound(UAudioComponent* AudioComp, float Fadeout)
{
	if(AudioComp)
	{
		if(AudioComp->IsPlaying())
		{
			AudioComp->FadeOut(Fadeout, 0.0f);
		}
	}
}

// Called to play a localized Environment sound
void ASoundEngine::PlayLocalizedEnvironmentSound(TEnumAsByte<EEnvironmentLocal> Event, FVector Location, UAudioComponent* AudioComp) 
{
	// All localized environment sounds
	if(AudioComp)
	{
		switch(Event)
		{
		case 0:
			// Fire Crackling
			if(FireCrackling)
				AudioComp->SetSound(FireCrackling);
			AudioComp->FadeIn(0.5);
			//AudioComp->Play();
			break;
		default:
			// Not real localized environment 
			break;
		}
	}
	else
	{
		switch(Event)
		{
		case 0:
			// Fire Crackling
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireCrackling, Location);
			break;
		default:
			// Not real localized environment 
			break;
		}
	}
}

// Called to play a global Environment sound
void ASoundEngine::PlayGlobalEnvironmentSound(TEnumAsByte<EEnvironmentGlobal> Event) 
{
	// All global environment sounds
	switch(Event)
	{
	case 0:
		// Wind
		UGameplayStatics::PlaySound2D(GetWorld(), Wind);
		break;
	default:
		// Not real global environment sound
		break;
	}
}

#pragma endregion

void ASoundEngine::SpawnSound(USoundBase* Sound, FVector Location, FRotator Rotation, float MaxRadius, float MinRadius) const
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const AAmbientSound* AmbientSound  = GetWorld()->SpawnActor<AAmbientSound>(Location, Rotation, SpawnInfo);
	AmbientSound->GetAudioComponent()->SetSound(Sound);
	AmbientSound->GetAudioComponent()->bOverrideAttenuation = true;
	FSoundAttenuationSettings Attenuation = FSoundAttenuationSettings();
	Attenuation.RadiusMax_DEPRECATED = MaxRadius;
	Attenuation.RadiusMin_DEPRECATED = MinRadius;
	Attenuation.bEnableOcclusion = true;
	AmbientSound->GetAudioComponent()->AdjustAttenuation(Attenuation);
}

#pragma region EnemyActions


// Called to play the correct Flyer action
void ASoundEngine::FlyerAction(TEnumAsByte<EAction> Action, FVector Location, UAudioComponent* AudioComp) const 
{
	// All Flyer actions
	if(AudioComp)
	{
		switch(Action)
		{
		case 0:
			// Attack
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), FlyerAttack, Location);
			if(AudioComp)
			{
				if(FlyerAttack)
				{
					AudioComp->SetSound(FlyerAttack);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		case 1:
			// Move
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), FlyerMove, Location);
			if(AudioComp)
			{
				if(FlyerMove)
				{
					AudioComp->SetSound(FlyerMove);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		case 2:
			// Death
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), FlyerDeath, Location);
			if(AudioComp)
			{
				if(FlyerDeath)
				{
					AudioComp->SetSound(FlyerDeath);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		default:
			// Not real action
			break;
		}
	}
	else
	{
		switch(Action)
		{
		case 0:
			// Attack
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FlyerAttack, Location);
			break;
		case 1:
			// Move
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FlyerMove, Location);
			break;
		case 2:
			// Death
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FlyerDeath, Location);
			break;
		default:
			// Not real action
			break;
		}
	}
}

// Called to play the correct Suicider action
void ASoundEngine::SuiciderAction(TEnumAsByte<EAction> Action, FVector Location, UAudioComponent* AudioComp) const 
{

	// Al Suicider actions
	if(AudioComp)
	{
		switch(Action)
		{
		case 0:
			// Attack
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SuiciderAttack, Location);
			if(AudioComp)
			{
				if(SuiciderAttack)
				{
					AudioComp->SetSound(SuiciderAttack);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		case 1:
			// Move
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SuiciderMove, Location);
			if(AudioComp)
			{
				if(SuiciderMove)
				{
					AudioComp->SetSound(SuiciderMove);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		case 2:
			// Death
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), SuiciderDeath, Location);
			if(AudioComp)
			{
				if(SuiciderDeath)
				{
					AudioComp->SetSound(SuiciderDeath);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		default:
			// Not real action
			break;
		}
	}
	else
	{
		switch(Action)
		{
		case 0:
			// Attack
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SuiciderAttack, Location);
			break;
		case 1:
			// Move
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SuiciderMove, Location);
			break;
		case 2:
			// Death
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SuiciderDeath, Location);
			break;
		default:
			// Not real action
			break;
		}
	}
}

// Called to play the correct Engineer action
void ASoundEngine::EngineerAction(TEnumAsByte<EAction> Action, FVector Location, UAudioComponent* AudioComp) const 
{
	// All Engineer actions
	if(AudioComp)
	{
		switch(Action)
		{
		case 0:
			// Attack
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), EngineerAttack, Location);
			if(AudioComp)
			{
				if(EngineerAttack)
				{
					AudioComp->SetSound(EngineerAttack);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		case 1:
			// Move
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), EngineerMove, Location);
			if(AudioComp)
			{
				if(EngineerMove)
				{
					AudioComp->SetSound(EngineerMove);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		case 2:
			// Death
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), EngineerDeath, Location);
			if(AudioComp)
			{
				if(EngineerDeath)
				{
					AudioComp->SetSound(EngineerDeath);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		default:
			// Not real action
			break;
		}
	}
	else
	{
		switch(Action)
		{
		case 0:
			// Attack
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EngineerAttack, Location);
			break;
		case 1:
			// Move
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EngineerMove, Location);
			break;
		case 2:
			// Death
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), EngineerDeath, Location);
			break;
		default:
			// Not real action
			break;
		}
	}
}

// Called to play the correct Grunt action
void ASoundEngine::GruntAction(TEnumAsByte<EAction> Action, FVector Location, UAudioComponent* AudioComp) const 
{
	// All Grunt actions
	if(AudioComp)
	{
		switch(Action)
		{
		case 0:
			// Attack
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), GruntAttack, Location);
			if(AudioComp)
			{
				if(GruntAttack)
				{
					AudioComp->SetSound(GruntAttack);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		case 1:
			// Move
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), GruntMove, Location);
			if(AudioComp)
			{
				if(GruntMove)
				{
					AudioComp->SetSound(GruntMove);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		case 2:
			// Death
			//UGameplayStatics::PlaySoundAtLocation(GetWorld(), GruntDeath, Location);
			if(AudioComp)
			{
				if(GruntDeath)
				{
					AudioComp->SetSound(GruntDeath);
				}
				AudioComp->FadeIn(0.5);
				//AudioComp->Play();
			}
			break;
		default:
			// Not real action
			break;
		}
	}
	else
	{
		switch(Action)
		{
		case 0:
			// Attack
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GruntAttack, Location);
			break;
		case 1:
			// Move
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GruntMove, Location);
			break;
		case 2:
			// Death
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GruntDeath, Location);
			break;
		default:
			// Not real action
			break;
		}
	}
}

#pragma endregion

#pragma region VolumeControl
// Called to mute all sounds
void ASoundEngine::StopAllSounds(bool FadeOut) const
{
	SetMasterVolume(0.0f);
}

// Called to mute all music
void ASoundEngine::StopAllMusic(bool FadeOut) const
{
	SetMusicVolume(0.0f);
}

// Called to mute all SFX sounds
void ASoundEngine::StopAllSFX(bool FadeOut) const
{
	SetSFXVolume(0.0f);
}

// Called to mute all UI sounds
void ASoundEngine::StopAllUI(bool FadeOut) const
{
	SetUIVolume(0.0f);
}

// Called to set Master Volume
void ASoundEngine::SetMasterVolume(float NewVolume) const
{
	MasterClass->Properties.Volume = NewVolume;
}

// Called to set Music Volume
void ASoundEngine::SetMusicVolume(float NewVolume) const 
{
	MusicClass->Properties.Volume = NewVolume;
}

// Called to set SFX Volume
void ASoundEngine::SetSFXVolume(float NewVolume) const
{
	SFXClass->Properties.Volume = NewVolume;
}

// Called to set UI Volume
void ASoundEngine::SetUIVolume(float NewVolume) const
{
	UIClass->Properties.Volume = NewVolume;
}

// Called to set UI Volume
void ASoundEngine::ResetAudioLevels() const
{
	SetMasterVolume(1.0f);
	SetMusicVolume(1.0f);
	SetSFXVolume(1.0f);
	SetUIVolume(1.0f);
}

#pragma endregion