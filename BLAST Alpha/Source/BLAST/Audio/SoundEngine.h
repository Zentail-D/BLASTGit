// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "SoundEngine.generated.h"


#pragma region ENUMS
/**
* Enum for the AI types
* Flyer - 0 Suicider - 1 Engineer - 2 Grunt - 3
*/
UENUM(BlueprintType)
enum EType
{
	Flyer        UMETA(DisplayName = "Flyer"),
	Suicider       UMETA(DisplayName = "Suicider"),
	Engineer   UMETA(DisplayName = "Engineer"),
	Grunt   UMETA(DisplayName = "Grunt"),
};

/**
* Enum for the actions
* Attacking - 0 Moving - 1 Dying - 2
*/
UENUM(BlueprintType)
enum EAction
{
	Attacking     UMETA(DisplayName = "Attacking"),
	Moving      UMETA(DisplayName = "Moving"),
	Dying   UMETA(DisplayName = "Dying"),
};

/**
* Enum for the weapon types
* Default - 0  Shotgun - 1 RocketLauncher - 2 Beam - 3 FlameThrower - 4 Charge - 5
*/
UENUM(BlueprintType)
enum EWeapon
{
	Default		UMETA(DisplayName = "Default"),
	Shotgun     UMETA(DisplayName = "Shotgun"),
	RocketLauncher      UMETA(DisplayName = "RocketLauncher"),
	Beam   UMETA(DisplayName = "Beam"),
	FlameThrower   UMETA(DisplayName = "FlameThrower"),
	Charge   UMETA(DisplayName = "Charge"),
};

/**
* Enum for the pickup types
* Weapon - 0 Part - 1
*/
UENUM(BlueprintType)
enum EPickup
{
	Weapon     UMETA(DisplayName = "Weapon"),
	Part      UMETA(DisplayName = "Part"),
};


/**
* Enum for the localized environment sounds
* FireCrackling - 0
*/
UENUM(BlueprintType)
enum EEnvironmentLocal
{
	FireCrackling     UMETA(DisplayName = "FireCrackling"),
};

/**
* Enum for the global environment sounds
* Wind - 0
*/
UENUM(BlueprintType)
enum EEnvironmentGlobal
{
	Wind     UMETA(DisplayName = "Wind"),
};

#pragma endregion

/** The Sound Engine */
UCLASS()
class BLAST_API ASoundEngine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASoundEngine();

#pragma region variables
	/** True - Subtitles will be shown. False - Subtitles will not be shown. */
	UPROPERTY(BlueprintReadWrite)
	bool bSubtitles = false;
#pragma endregion 

#pragma region Classes
	/** Master Sound Class for all sounds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Class")
	USoundClass* MasterClass;
	/** Sound Class for all SFX sounds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Class")
	USoundClass* SFXClass;
	/** Sound Class for all UI sounds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Class")
	USoundClass* UIClass;
	/** Sound Class for all Music. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Class")
	USoundClass* MusicClass;
#pragma endregion 
	
#pragma region SoundCues
	
#pragma region Player
	/** Sound Cue for player jumping. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Player")
	USoundCue* PlayerJump;
	/** Sound Cue for player landing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Player")
	USoundCue* PlayerLand;
	/** Sound Cue for player death. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Player")
	USoundCue* PlayerDeath;
	/** Sound Cue for player mantling. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Player")
	USoundCue* PlayerMantle;
	/** Sound Cue for player walking. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Player")
	USoundCue* PlayerWalk;
	/** Sound Cue for player running. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Player")
	USoundCue* PlayerRun;
	/** Sound Cue for player crouching. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Player")
	USoundCue* PlayerCrouch;
	/** Sound Cue for player walking while crouching. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Player")
	USoundCue* PlayerCrouchWalk;
	/** Sound Cue for player interacting. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Player")
	USoundCue* PlayerInteract;
#pragma endregion
	
#pragma region Enemies
#pragma region Suicider
	/** Sound Cue for Suicider moving. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Suicider")
	USoundCue* SuiciderMove;
	/** Sound Cue for Suicider death. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Suicider")
	USoundCue* SuiciderDeath;
	/** Sound Cue for Suicider attacking. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Suicider")
	USoundCue* SuiciderAttack;
#pragma endregion 
#pragma region Grunt
	/** Sound Cue for Grunt moving. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Grunt")
	USoundCue* GruntMove;
	/** Sound Cue for Grunt death. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Grunt")
	USoundCue* GruntDeath;
	/** Sound Cue for Grunt attacking. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Grunt")
	USoundCue* GruntAttack;
#pragma endregion 
#pragma region Flyer
	/** Sound Cue for Flyer moving. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Flyer")
	USoundCue* FlyerMove;
	/** Sound Cue for Flyer death. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Flyer")
	USoundCue* FlyerDeath;
	/** Sound Cue for Flyer attacking. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Flyer")
	USoundCue* FlyerAttack;
#pragma endregion 
#pragma region Engineer
	/** Sound Cue for Engineer moving. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Engineer")
	USoundCue* EngineerMove;
	/** Sound Cue for Engineer death. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Engineer")
	USoundCue* EngineerDeath;
	/** Sound Cue for Engineer attacking. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Enemies|Engineer")
	USoundCue* EngineerAttack;
#pragma endregion
#pragma endregion
	
#pragma region Pickups
	/** Sound Cue for a Part dropping. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Pickups")
	USoundCue* PartDropped;
	/** Sound Cue for a Part picked up. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Pickups")
	USoundCue* PartPickup;
	/** Sound Cue for a Weapon dropping. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Pickups")
	USoundCue* WeaponDropped;
	/** Sound Cue for a Weapon picked up. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Pickups")
	USoundCue* WeaponPickup;
#pragma endregion
	
#pragma region Weapons
	/** Sound Cue for a Default weapon firing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Weapons")
	USoundCue* DefaultFire;
	/** Sound Cue for a Shotgun firing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Weapons")
	USoundCue* ShotgunFire;
	/** Sound Cue for a Rocket Launcher firing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Weapons")
	USoundCue* RocketLauncherFire;
	/** Sound Cue for a Flame Thrower firing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Weapons")
	USoundCue* FlameThrowerFire;
	/** Sound Cue for a Beam firing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Weapons")
	USoundCue* BeamFire;
	/** Sound Cue for a Weapon firing. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|SFX|Weapons")
	USoundCue* ChargeFire;
#pragma endregion
	
#pragma region Environment
	/** Sound Cue for Wind. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Environment")
	USoundCue* Wind;
	/** Sound Cue for a Generator on. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Environment")
	USoundCue* GeneratorOn;
	/** Sound Cue for Fire Crackling. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Environment")
	USoundCue* FireCrackling;
	/** Sound Cue for Sparks. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Environment")
	USoundCue* Sparks;
#pragma endregion
	
#pragma region Menu
	/** Sound Cue for a Button Clicked. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|UI")
	USoundCue* ButtonClicked;
#pragma endregion
	
#pragma region Announcer
	/** Sound Cue to announce Generator on. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Voice|Announcer")
	USoundCue* AnnouncerGenOn;
	/** Sound Cue to announce Win. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Voice|Announcer")
	USoundCue* AnnouncerWin;
	/** Sound Cue to announce Loss. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Voice|Announcer")
	USoundCue* AnnouncerLose;
#pragma endregion
	
#pragma region Music
	/** Sound Cue for Main Menu Music. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Music")
	USoundCue* MainMenu;
	/** Sound Cue for Victory Music. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Music")
	USoundCue* Victory;
	/** Sound Cue for Loss Music. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Music")
	USoundCue* Loss;
	/** Sound Cue for Background Music. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio|Music")
	USoundCue* BackgroundMusic;
#pragma endregion
	
#pragma endregion 
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called when the game end or when removed
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:

	/**
	* Toggles subtitles on/off.
	*/
	UFUNCTION(BlueprintCallable)
	void ToggleSubtitles();


#pragma region CalledFunctions
	/**
	* Plays the given Action for the given Enemy at the given Location.
	* @param AIName The enemy type
	* @param Action The action to be played
	* @param Location The location to play the sound if no AudioComp is passed
	* @param AudioComp The component to play the sound
	*/
	UFUNCTION(BlueprintCallable)
	void PlayAISound(TEnumAsByte<EType> AIName, TEnumAsByte<EAction> Action, FVector Location = FVector(0,0,0), UAudioComponent* AudioComp = nullptr);
	
	
	/**
	* Plays the given Weapon's Fire sound at the given Location.
	* @param WeaponName The weapon type to be played
	* @param Location The location to play the sound if no AudioComp is passed
	* @param AudioComp The component to play the sound
	*/
	UFUNCTION(BlueprintCallable)
	void PlayWeaponSound(TEnumAsByte<EWeapon> WeaponName, FVector Location = FVector(0,0,0), UAudioComponent* AudioComp = nullptr);
	
	
	/**
	* Play the Pickup Drop sound.
	* @param Pickup Pickup type that was dropped.
	* @param Location The location to play the sound if no AudioComp is passed
	* @param AudioComp The component to play the sound
	*/
	UFUNCTION(BlueprintCallable)
	void PlayPickupDropSound(TEnumAsByte<EPickup> Pickup, FVector Location = FVector(0,0,0), UAudioComponent* AudioComp = nullptr);


	/**
	* Play the Pickup Pickup sound.
	* @param Pickup Pickup type that was picked up.
	* @param Location The location to play the sound if no AudioComp is passed
	* @param AudioComp The component to play the sound
	*/
	UFUNCTION(BlueprintCallable)
	void PlayPickupPickupSound(TEnumAsByte<EPickup> Pickup, FVector Location = FVector(0,0,0), UAudioComponent* AudioComp = nullptr);
	
	
	/**
	* Stop playing the sound.
	* @param AudioComp Component to stop the sound of.
	* @param Fadeout Time it'll take to fade out the sound.
	*/
	UFUNCTION(BlueprintCallable)
	void StopSound(UAudioComponent* AudioComp, float Fadeout);
	

	/**
	* Play a Localized Environment sound.
	* @param Event The sound to play.
	* @param Location The location to play the sound if no AudioComp is passed
	* @param AudioComp The component to play the sound
	*/
	UFUNCTION(BlueprintCallable)
	void PlayLocalizedEnvironmentSound(TEnumAsByte<EEnvironmentLocal> Event, FVector Location = FVector(0,0,0), UAudioComponent* AudioComp = nullptr );
	

	/**
	* Play a Global Environment sound.
	* @param Event The sound to play.
	*/
	UFUNCTION(BlueprintCallable)
	void PlayGlobalEnvironmentSound(TEnumAsByte<EEnvironmentGlobal> Event);
	
	
#pragma endregion 	

	
#pragma region Protected
protected:

	/**
	* Spawns a new AmbientSound to the world
	* @param Sound The sound the AmbientSound will
	* @param Location The location where the AmbientSound will be spawned
	* @param Rotation The rotation the AmbientSound will have
	* @param MaxRadius The Max Radius to hear the sound
	* @param MinRadius The Min Radius to hear the sound
	*/
	void SpawnSound(USoundBase* Sound, FVector Location, FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f), float MaxRadius = 3600.0f, float MinRadius = 400.0f) const;
	
	/**
	* Plays the correct Flyer Action.
	* @param AudioComp The component to play the sound
	* @param Action The action to be played
	*/
	void FlyerAction(TEnumAsByte<EAction> Action, FVector Location = FVector(0,0,0), UAudioComponent* AudioComp = nullptr) const;

	/**
	* Plays the correct Suicider Action.
	* @param AudioComp The component to play the sound
	* @param Action The action to be played
	*/
	void SuiciderAction(TEnumAsByte<EAction> Action, FVector Location = FVector(0,0,0), UAudioComponent* AudioComp = nullptr) const;

	/**
	* Plays the correct Engineer Action.
	* @param AudioComp The component to play the sound
	* @param Action The action to be played
	*/
	void EngineerAction(TEnumAsByte<EAction> Action, FVector Location = FVector(0,0,0), UAudioComponent* AudioComp = nullptr) const;

	/**
	* Plays the correct Grunt Action.
	* @param AudioComp The component to play the sound
	* @param Action The action to be played
	*/
	void GruntAction(TEnumAsByte<EAction> Action, FVector Location = FVector(0,0,0), UAudioComponent* AudioComp = nullptr) const;
	
#pragma endregion 


#pragma region VolumeControls
public:
	
	/**
	* Stops all playing sounds.
	* @param FadeOut True - fade the sounds out. False - stop sounds immediately.     
	*/
	UFUNCTION(BlueprintCallable)
	void StopAllSounds(bool FadeOut) const;

	/**
	* Stops all playing music.
	* @param FadeOut True - fade the sounds out. False - stop sounds immediately.     
	*/
	UFUNCTION(BlueprintCallable)
	void StopAllMusic(bool FadeOut) const;
	
	/**
	* Stops all playing sfx sounds.
	* @param FadeOut True - fade the sounds out. False - stop sounds immediately.     
	*/
	UFUNCTION(BlueprintCallable)
	void StopAllSFX(bool FadeOut) const;

	/**
	* Stops all playing UI sounds.
	* @param FadeOut True - fade the sounds out. False - stop sounds immediately.     
	*/
	UFUNCTION(BlueprintCallable)
	void StopAllUI(bool FadeOut) const;

	
	/**
	* Changes the Master Volume.
	* @param NewVolume The new value for the Master Volume. Range: [0.0 ... 1.0]       
	*/
	UFUNCTION(BlueprintCallable)
	void SetMasterVolume(float NewVolume) const;

	/**
	* Changes the Music Volume.
	* @param NewVolume The new value for the Music Volume. Range: [0.0 ... 1.0]    
	*/
	UFUNCTION(BlueprintCallable)
	void SetMusicVolume(float NewVolume) const;

	/**
	* Changes the SFX Volume.
	* @param NewVolume The new value for the SFX Volume. Range: [0.0 ... 1.0]   
	*/
	UFUNCTION(BlueprintCallable)
	void SetSFXVolume(float NewVolume) const;

	/**
	* Changes the UI Volume.
	* @param NewVolume The new value for the UI Volume. Range: [0.0 ... 1.0]       
	*/
	UFUNCTION(BlueprintCallable)
	void SetUIVolume(float NewVolume) const;

	/**
	* Resets the all volume levels     
	*/
	UFUNCTION(BlueprintCallable)
	void ResetAudioLevels() const;
	
#pragma endregion
	
};
