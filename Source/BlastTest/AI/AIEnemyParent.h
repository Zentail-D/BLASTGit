// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationInvokerComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "PatrollingPath.h"
#include "EBlueprintEnums.h"
#include "AIEnemyParent.generated.h"

UCLASS()
class  AAIEnemyParent : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AAIEnemyParent();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	* Niagara Component that handles all of the VFX set this in blueprints and activate through blueprints
	*/
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = Niagara)
	UNiagaraComponent* NiagaraParticleSystem;

	/**
	* Invoker Component that allows the Navigation to be generated in real time  
	*/
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = InvokerComponent)
	UNavigationInvokerComponent* InvokerComponent;

	/**
	 * Drop component handles the drops for the mods that ai drop when destroyed
	 */
	//UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = InvokerComponent)
	//UDropItemComponent* DropItemComponent;
	
	/**
	 * Reference to all the Player in the Array
	 * @return TArray of Pointers to Player Characters
	 */
	TArray<ACharacter*> GetPlayerCharacters();

	/**
	 * Reference to the patrol path contained in the Enemy
	 * @return APatrollingPath that holds a vector to the path the enemy should patrol
	 */
	APatrollingPath* GetPatrollingPath();

	/**
	 * Setter for PatrollingPath
	 * @param NewPatrollingPath set the NewPatrollingPath to the path that the Enemy is patrolling
	 */
	void SetPatrollingPath(APatrollingPath* NewPatrollingPath);

	/**
	 * Setter for the PlayerCharacters
	 * @param NewPlayerCharacters Set the player character that the Enemy are targeting to this new TArray of Characters
	 */
	void SetPlayerCharacters(TArray<ACharacter*> NewPlayerCharacters);

	/**
	 *@return true if there is atleast one player in the playercharacters array
	 */
	inline bool HasPlayerToAttack() const { return PlayerCharacters.Num() > 0; }

	/**
	 *Fire a projectile of ProjectileClass at the Player that is selected from playerCharacterIndex with some prediction on firing
	 *@param PlayerCharacterIndex THe player that the fire should be shooting at
	 */
	void Fire(int PlayerCharacterIndex);

	/**
	 * Calls the freeze behavior in the controller
	 * @return Return a boolean true if successfully freezed the character and false if the character was already frozen
	 */
	bool FreezeBehavior();

	/**
	* Calls the unfreeze behavior in the controller
	* @return Return a boolean true if successfully unfreeze the character and false if the character was already unfrozen
	*/
	bool UnFreezeBehavior();

	/**
	 * sets the current health to the NewHealth
	 * @param Damage The amount to reduce current health by
	 */
	UFUNCTION(BlueprintCallable)
	void DealDamageToEnemy(int Damage);

	/**
	 * @return  Gives amount of damage enemy deals
	 */
	int GetDamageAmount();

	/**
	* Call to add another muzzle location to enemy to shoot from. Initialize the Muzzle in the contructor and pass it into this function and the fire function will loop through the different muzzle locations
	* @param NewMuzzleLocation StaticMeshComponent that is set to be invisible in game and has no collision. Used to inform the bullets where to spawn in at
	*/
	void AddMuzzleLocation(UStaticMeshComponent* NewMuzzleLocation);

	/**
	 * Getter for the attacking cooldown
	 * @return Gives the attacking cooldown
	 * @warning It will only be set by the blueprint default values
	 */
	float GetAttackingCooldown();

	/**
	*Setter so that we can update the current health
	* @warning Typically you will only ever past DetlaTime to this function
	*/
	void DecreaseAttackingCooldown(float TimeToSubtract);

	/** Resets the attack cooldown back to the AttackCooldownReset */
	void ResetAttackingCooldown();

	/**
	* Getter for the Movement Speed
	* @return Gives the Movement Speed
	* @warning It will only be set by the blueprint default values
	*/
	float GetMovementSpeed();

	/**
	* Getter for the Current Health
	* @return Gives the Current Health
	* @warning It will only be set by the blueprint default values
	*/
	float GetCurrentHealth();
	
	/**
	* Getter for the Total Health
	* @return Gives the Total Health
	* @warning It will only be set by the blueprint default values
	*/
	float GetTotalHealth();

	/**
	* Getter for the Score Amount
	* @return Gives the Score Amount
	* @warning It will only be set by the blueprint default values
	*/
	int GetScoreAmount();

	/**
	* Getter for the Move To Tolerance
	* @return Gives the Move To Tolerance
	* @warning It will only be set by the blueprint default values
	*/
	int GetMoveToTolerance();

	/**
	* Getter for the Player Found Tolerance
	* @return Gives the Player Found Tolerance
	* @warning It will only be set by the blueprint default values
	*/
	int GetPlayerFoundTolerance();

	/**
	* Getter for the Attacking Range
	* @return Gives the Attacking Range
	* @warning It will only be set by the blueprint default values
	*/
	float GetAttackingRange();

	/** ----------------- STATUS EFFECT SECTION START (TALK TO NICK TO EDIT) ------------------ */

	/** Decreases the CurrentFireStatusEffectDuration by delta time
	* @return - True if CurrentFireStatusEffectDuration is now below zero false if not
	*/
	bool DecrementFireStatusEffectDuration(float DeltaTime);

	/** resets the fire status effect duration to the max timer */
	inline void ResetFireStatusEffectDuration() { CurrentFireStatusEffectDuration = FireStatusEffectDuration; }

	/** Decreases the CurrentFireStatusEffectDamageFrequencyTimer by delta time
	* @return - True if CurrentFireStatusEffectDamageFrequencyTimer is now below zero, false if not
	*/
	bool DecrementFireStatusEffectDamageFrequency(float DeltaTime);

	/** resets the first status effect damage frequency timer to its max timer */
	inline void ResetFireStatusEffectDamageFrequencyTimer() { CurrentFireStatusEffectDamageFrequencyTimer = FireStatusEffectDamageFrequency; }
	
	/** ---- SETTER ---- */

	/** Called anywhere to set the enemy status effect
	* @param Status - This is the status from the enemy status effect enum for what status to apply
	*/
	void SetEnemyStatusEffect( const EWeaponStatusEffects Status);

	/** ---- GETTERS ---- */
	
	/** Possibly useful getter for retrieving the current status effect the enemy has
	* @return - the status effect the enemy has, if NONE there is no current status effect
	*/
	inline EWeaponStatusEffects GetCurrentEnemyStatusEffects() const { return CurrentEnemyStatusEffect; }

	/** ----------------- STATUS EFFECT SECTION END ------------------ */
	
protected:
	/**Called when the game starts or when spawned*/
	virtual void BeginPlay() override;

	/** ----------------- STATUS EFFECT SECTION START (TALK TO NICK TO EDIT) ------------------ */
	
	/** The current status that the enemy has on them, the details of this will be handled in the tick function
	* on a switch statement
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "AI Settings | Status Effect Settings")
	EWeaponStatusEffects CurrentEnemyStatusEffect = EWeaponStatusEffects::Ve_None;

	/** How long the fire effect will last in total */
	UPROPERTY(EditAnywhere, Category = "AI Settings | Status Effect Settings | Fire Status", meta= (AllowPrivateAccess= "true"))
	float FireStatusEffectDuration = 5.0f;

	/** The var that will actually be uses as the timer to get decremented for the fire status effect duration */
	float CurrentFireStatusEffectDuration = FireStatusEffectDuration;

	/** How often the fire effect will damage the Enemy */
	UPROPERTY(EditAnywhere, Category = "AI Settings | Status Effect Settings | Fire Status", meta= (AllowPrivateAccess= "true"))
	float FireStatusEffectDamageFrequency = 1.0f;

	/** The var that will actually be uses as the timer to get decremented for the damage frequency of the fire status*/
	float CurrentFireStatusEffectDamageFrequencyTimer = 0.f;
	
	/** How much damage the fire effect will do (frequency of damaged is based on FireStatusEffectDamageFrequency) */
	UPROPERTY(EditAnywhere, Category = "AI Settings | Status Effect Settings | Fire Status", meta= (AllowPrivateAccess= "true"))
	float FireStatusEffectDamage = 1.0f;

	/** ----------------- STATUS EFFECT SECTION END ------------------ */

	/**
	* Cool down for attacks
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	float AttackingCooldown;
	/**Backup of AttackingCooldown so that it can be reset*/
	float AttackCooldownReset;
	/**Speed the AI moves at*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	int MovementSpeed;
	/**Max amount of health the enemy can have*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | General Stats", meta = (AllowProtectedAccess = "true"))
	float TotalHealth;
	/**Amount of health the enemy currently has*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | General Stats", meta = (AllowProtectedAccess = "true"))
	float CurrentHealth;
	/**Amount of Damage the enemy can do*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Attack Settings", meta = (AllowProtectedAccess = "true"))
	int DamageAmount;
	/**Amount of score that a enemy will be worth for the leaderboard*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | General Stats", meta = (AllowProtectedAccess = "true"))
	int ScoreAmount;
	/**The tolerance for any MoveTo calls in the behavior tree for this enemy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	int MoveToTolerance;
	/**Tolerance for the PlayerFound MoveTo calls in the behavior tree*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Movement Settings", meta = (AllowProtectedAccess = "true"))
	int PlayerFoundTolerance;
	/**Cool down used for when a enemy shoots multiple bullets. Amount of time between each shot*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Projectile Settings", meta = (AllowProtectedAccess = "true"))
	float MultiShotCooldown;
	/**Range the Enemy can attack in*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Projectile Settings", meta = (AllowProtectedAccess = "true"))
	float AttackingRange;
	/**Speed of any bullets fired by this enemy*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Projectile Settings", meta = (AllowProtectedAccess = "true"))
	float BulletSpeed;
	/**Enemy type identifier to be used to get the proper loot table when dieing
	 *@warning Must be set in each child
	 */
	FString EnemyType;
	/**if 0 not on fire, otherwise, its how long left till not on fire */
	float FireStatusDuration;
	/**if 0 not on fire, otherwise, its how long left till not on fire */
	float FireDamage;
private:
	/**Patrolling Path that the enemy follows. Must be set for the AI to run behavior tree. Can be set manually or by using spawners*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Movement Settings", meta = (AllowPrivateAccess = "true"))
	APatrollingPath* PatrollingPath;

	/**An Array of Targets that the enemy can fire at and  discover. Must be set to run behavior tree*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Movement Settings", meta = (AllowPrivateAccess = "true"))
	TArray<ACharacter*> PlayerCharacters;

	/**Is the character frozen. If true then it is frozen. False for it is not frozen*/
	bool bFrozen;

	/**Class that the projectiles are fired using*/
	UPROPERTY(EditAnywhere, Category = "AI Settings | Projectile Settings", meta= (AllowPrivateAccess= "true"))
	TSubclassOf<class AEnemyProjectile> ProjectileClass;

	/**Locations for the muzzle or the spawn location for bullets being fired. Can be more than one and index using MuzzleLocationIndex*/
	UPROPERTY(EditAnywhere, Category = "AI Settings | Projectile Settings", meta= (AllowPrivateAccess= "true"))
	TArray<UStaticMeshComponent*> MuzzleLocations;

	/**Index for the MuzzleLocation to tell which location to use for the bullet*/
	UPROPERTY(EditAnywhere, Category = "AI Settings | Projectile Settings", meta= (AllowPrivateAccess= "true"))
	int MuzzleLocationIndex;

	/**
	 * Called inside the Parent for Enemies that are destroying themselves that need to inform other enties that they are destroying themselves
	 */
	virtual void DestroyChildren();

	/**Incrementing MuzzleLocationIndex by one and if it is already at the end of the Array will reset the index to 0*/
	void IncrementMuzzleIndex();
};
