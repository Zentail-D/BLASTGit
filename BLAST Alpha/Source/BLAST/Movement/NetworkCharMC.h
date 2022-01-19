// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EWallRunSide.h"
#include "GameFramework/CharacterMovementComponent.h"
//#include "NetworkCharCameraShake.h"
#include "NetworkCharMC.generated.h"

/**
 * class for the movement networking of both players
 */
UCLASS(Blueprintable)
class BLAST_API UNetworkCharMC : public UCharacterMovementComponent
{
	GENERATED_BODY()
	/** this is a helper class that sends flags over the server */
	friend class FSavedMove_My;

#pragma region Defaults
private:
	/** The ground speed when running*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float RunSpeed = 450.0f;
	/** The ground speed when sprinting*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 925.0f;
	/** The ground speed when sliding*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float BaseSlideSpeed = 1000.0f;
	/** The ground speed when sliding to reset*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float SlideSpeed = BaseSlideSpeed;
	/** The rate at which the sliding speed increases as you slide downhill*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float SlideFlatDeceleration = 750.0f;
	/** The rate at which the sliding speed increases as you slide downhill*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float SlideDownhillAcceleration = 500.0f;
	/** The rate at which the sliding speed increases as you slide downhill*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float SlideUphillDeceleration = 850.0f;
	/** Slide flag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	mutable bool bIsSlide = false;
	/** On ground flag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	mutable bool bOnGround = false;
	/**The acceleration when running*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float RunAcceleration = 2000.0f;
	/** The acceleration when sprinting*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float SprintAcceleration = 2000.0f;
	/** The amount of vertical room between the two line traces when checking if the character is still on the wall*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Wall Running", Meta = (AllowPrivateAccess = "true"))
	float LineTraceVerticalTolerance = 50.0f;
	/** The wall's normal when the player vaults on it */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Wall Running", Meta = (AllowPrivateAccess = "true"))
	FVector WallNormal;
	/** Checks the angle between player and wall and determines if they can climb it */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Wall Running", Meta = (AllowPrivateAccess = "true"))
	bool CheckAngle = true;
	/** The player's velocity while wall running*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Wall Running", Meta = (AllowPrivateAccess = "true"))
	float VaultRunSpeed = 600.0f;
	/** Vaulting Timer reset value*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Character Movement|Wall Running", Meta = (AllowPrivateAccess = "true"))
	float VaultTimerBase = 0.5f;
	/** Vaulting Timer*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Character Movement|Wall Running", Meta = (AllowPrivateAccess = "true"))
	float VaultTimer = VaultTimerBase;
	/** Locks movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Wall Running", Meta = (AllowPrivateAccess = "true"))
	bool bLockMovement = false;
	/** Checks if the player is crouching or not */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	bool bCrouch = false;
	FTimerHandle SlideTimerHandle;
	float SlideTimer = 1.0f;
	FVector newVelocity;
	/** The normal of the ground the player is standing on */
	FVector groundNormal;
	/** The direction the player slides*/
	FVector slideForward;
	//** Shakes the camera based on player's movement */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMatineeCameraShake> NetworkCameraShake;
	
#pragma endregion

#pragma region Sprinting Functions
public:
	/** Sets sprinting to either enabled or disabled */
	UFUNCTION(BlueprintCallable, Category = "My Character Movement")
	void SetSprinting(bool sprinting);
	bool ReturnCrouch() const;
	// UFUNCTION(BlueprintCallable, Category = "My Character Movement")
	// void SetCrouch(bool set);
	bool ReturnMoveLock() const;
	FVector GetSlideVelocity() const;
	UFUNCTION(BlueprintCallable, Category = "My Character Movement")
	void SetAirControl(int a);
	/** Starts the walking camera shake */
	void BeginWalkCameraShake();
	/** Ends the walking camera shake. */
	void EndWalkCameraShake();
#pragma endregion

#pragma region Wall Running Functions
	/** Requests that the character begins vault. Will return false if the required keys are not being pressed*/
	UFUNCTION(BlueprintCallable, Category = "Custom Character Movement")
	bool BeginVaultingRun();
	/** Ends the character's vault */
	UFUNCTION(BlueprintCallable, Category = "Custom Character Movement")
	void EndVaultingRun();
	/** Returns true if the required vaulting keys are currently down*/
	bool AreRequiredVaultingRunKeysDown() const;
	/**
	 *Returns true if the player is next to a wall that can be vaulted
	 * @param vertical_tolerance The tolerance of how vertical we have to be to do a vault or mantle
	 */
	bool IsNextToWall(float vertical_tolerance = 0.0f);
	/**
	*Returns true if the angle between the player and the wall is small enough to start climbing it
	*/
	bool CheckAngleToWall();
	/**
	*Returns true if the player is on the ground
	*/
	bool IsOnGround();
	/**
	 *Finds the vaulting direction and side based on the specified surface normal
	 * @param surface_normal the normal of the wall itself
	 * @param direction the direction of the wall
	 * @param side side of the wall the character is hitting
	 */
	void FindVaultingDirectionAndSide(const FVector& surface_normal, FVector& direction, EWallRunSide& side) const;
	/**
	 *Helper function that returns true if the specified surface normal can be vaulted on
	 * @param surface_normal of the wall checking if we can mantle or vault
	 */
	bool CanSurfaceBeVaulting(const FVector& surface_normal) const;
	/**
	 *Returns true if the movement mode is custom and matches the provided custom movement mode
	 *@param custom_movement_mode movement mode we are checking if they are custom or not
	 */
	bool IsCustomMovementMode(uint8 custom_movement_mode) const;
private:
	/**
	 *Called when the owning actor hits something (to begin the wall run)
	 * @param SelfActor The owning actor
	 * @param OtherActor The actor the owning actor collides with
	 * @param NormalImpulse force at which they hit each other
	 * @param Hit If they hit or not
	 */
	UFUNCTION()
	void OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
	/** This is the function that changes the camera every tick */
	void CameraTick() const;
	/**
	 *This is the function that changes the camera tilt every tick
	 * @param TargetRoll amount we want to tilt the camera
	 */
	void CameraTilt(float TargetRoll) const;
#pragma endregion
#pragma region Other Functions
	/**Returns true if the required mantle keys are currently down*/
	bool AreRequiredMantleRunKeysDown() const;
	/** Begin the characters Mantle*/
	UFUNCTION(BlueprintCallable, Category = "Custom Character Movement")
	bool BeginMantle();
	/** Ends the character's mantle */
	UFUNCTION(BlueprintCallable, Category = "Custom Character Movement")
	void EndMantle();
	/** Returns true if the required slide keys are currently down */
	bool AreRequiredSlideRunKeysDown() const;
	/** Begins the slide motion*/
	UFUNCTION(BlueprintCallable, Category = "Custom Character Movement")
	void BeginSlide();
	/** Ends the character's wall run*/
	UFUNCTION(BlueprintCallable, Category = "Custom Character Movement")
	void EndSlide();
#pragma endregion	
#pragma region Overrides
protected:
	/** BeginPlay is the first function to be called when playing*/
	virtual void BeginPlay() override;
	/**
	 * Actives whenever a component is destroyed
	 * @param bDestroyingHierarchy Boolean determining hierarchy of destruction
	 */
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
public:
    // UFUNCTION(BlueprintCallable, Category = "Custom Character Movement")
	// virtual void Crouch(bool bClientSimulation) override;
	// UFUNCTION(BlueprintCallable, Category = "Custom Character Movement")
	// virtual void UnCrouch(bool bClientSimulation) override;
	
	/**
	 * Updates component every tick of the game.
	 * @param DeltaTime The change in time each tick
	 * @param TickType The different type of tick and which one is used
	 * @param ThisTickFunction What kind of tick function this is
	 */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	/**
	 * Updates all of the current compressed flags
	 * @param Flags The item that is being updated
	 * @warning if you are NOT network programmer do not touch this code!!
	 */
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	/**
	 * Changes the previous movement mode
	 * @param PreviousMovementMode Previous movement mode we were under
	 * @param PreviousCustomMode Previous Custom movement mode we were under
	 */
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;
	/**
	 * Goes though which custom movement modes physics is being used
	 * @param deltaTime the change in time
	 * @param Iterations The iterations we go through for each physics variable
	 */
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	/**
	 * Goes through the proper physics for Vaulting
	 * @param deltaTime the change in time
	 * @param Iterations The iterations we go through for each physics variable
	 */
	void PhysVaulting(float deltaTime, int32 Iterations);
	FVector SlideVector();
	/**
	 *Goes through which custom movement modes physics is being used
	 *@param deltaTime - the change in time
	 *@param Iterations - The iterations we go through for each physics variable
	 */
	void PhysSliding(float deltaTime, int32 Iterations);
	void PhysMantle(float deltaTime, int32 Iterations);
	/** Gets and sets the max speed of the player*/
	virtual float GetMaxSpeed() const override;
	/** Gets and sets the max acceleration of the player*/
	virtual float GetMaxAcceleration() const override;
	/**
	 * Handles the event of when the player lands.
	 * @param Hit If we hit the ground or not
	 * @param remainingTime Time before we hit the ground
	 * @param Iterations The iterations we go through for the fall
	 */
	virtual void ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations) override;
	/**
	 * Predicts the data from the client to approve future movements
	 * @warning if you are NOT network programmer do not touch this code!!
	 */
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
#pragma endregion

#pragma region Compressed Flags
private:
	/**
	* Compressed flag that gets set when sprint keys are pressed
	* @warning if you are NOT network programmer do not touch this code!!
	*/
	uint8 WantsToSprint : 1;
	/**
	* Compressed flag that gets set when vaulting keys are pressed
	* @warning if you are NOT network programmer do not touch this code!!
	*/
	uint8 VaultingKeysDown : 1;
	/**
	* Compressed flag that gets set when slide keys are pressed
	* @warning if you are NOT network programmer do not touch this code!!
	*/
	uint8 WantsToSlide: 1;
	/**
	* Compressed flag that gets set when mantle keys are pressed
	* @warning if you are NOT network programmer do not touch this code!!
	*/
	uint8 MantleKeysDown : 1;
#pragma endregion

#pragma region Private Variables
	/**
	 *True if the sprint key is down
	 *@warning if you are NOT network programmer do not touch this code!!
	 */
	bool SprintKeyDown = false;
	/** The direction the character is currently wall running in*/
	FVector WallRunDirection;
	/** The side of the wall the player is running on.*/
	EWallRunSide WallRunSide;
#pragma endregion
};
/** This class handles the compress flags */
class FSavedMove_My : public FSavedMove_Character
{
public:

	typedef FSavedMove_Character Super;

	/**
	 * Resets all saved variables.
	 * @warning if you are NOT network programmer do not touch this code!!
	 */
	virtual void Clear() override;
	/**
	 * Store input commands in the compressed flags.
	 * @warning if you are NOT network programmer do not touch this code!!
	 */
	virtual uint8 GetCompressedFlags() const override;
	/**
	 *This is used to check whether or not two moves can be combined into one. Basically you just check to make sure that the saved variables are the same.
	 * @param NewMovePtr The new movement pointer
	 * @param Character The character we are using
	 * @param MaxDelta The max delta for multiple functions
	 * @warning if you are NOT network programmer do not touch this code!!
	 */
	virtual bool CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* Character, float MaxDelta) const override;
	/**
	 * Sets up the move before sending it to the server.
	 * @param Character Character we are using
	 * @param InDeltaTime Delta time to be combined with functions
	 * @param NewAccel The new acceleration
	 * @param ClientData Client data we are sending over
	 * @warning if you are NOT network programmer do not touch this code!!
	 */
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
	/**
	 * Sets variables on character movement component before making a predictive correction.
	 * @param Character The character we are using
	 * @warning if you are NOT network programmer do not touch this code!!
	 */
	virtual void PrepMoveFor(class ACharacter* Character) override;

private:
	/**
	* Compressed flag that get saved off and set over server when sprint keys are pressed
	* @warning if you are NOT network programmer do not touch this code!!
	*/
	uint8 SavedWantsToSprint : 1;
	/**
	* Compressed flag that get saved off and set over server when vault keys are pressed
	* @warning if you are NOT network programmer do not touch this code!!
	*/
	uint8 SavedVaultingKeysDown : 1;
	/**
	* Compressed flag that get saved off and set over server when slide keys are pressed
	* @warning if you are NOT network programmer do not touch this code!!
	*/
	uint8 SavedWantsToSlide: 1;
	/**
	* Compressed flag that get saved off and set over server when mantle keys are pressed
	* @warning if you are NOT network programmer do not touch this code!!
	*/
	uint8 SavedMantleKeysDown : 1;
};
/**
 * Class that predicts the data to properly handle the movement
 * @warning if you are NOT network programmer do not touch this code!!
 */
class FNetworkPredictionData_Client_My : public FNetworkPredictionData_Client_Character
{
public:
	typedef FNetworkPredictionData_Client_Character Super;

	/**
	 * Constructor for this helper class
	 * @param ClientMovement The client movement we are trying to predict
	 * @warning if you are NOT network programmer do not touch this code!!
	 */
	FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement);

	/**
	 * brief Allocates a new copy of our custom saved move
	 * @warning if you are NOT network programmer do not touch this code!!
	 */
	virtual FSavedMovePtr AllocateNewMove() override;
};