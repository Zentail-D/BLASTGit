// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPCustomMovement.generated.h"

/**
 * Class for the custom movement of the players
 */
UCLASS(Blueprintable)
class BLAST_API UFPCustomMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()

#pragma region Defult Values
private:
	/** The ground speed when sliding*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FP Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float BaseSlideSpeed = 1200.0f;
	
	/** The ground speed when sliding to reset*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FP Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float SlideSpeed = BaseSlideSpeed;
	
	/** Slide flag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FP Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	mutable bool bIsSlide = false;

	/** The rate at which the sliding speed increases as you slide downhill*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	float SlideFlatDeceleration = 750.0f;

	/** On ground flag */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Character Movement|Grounded", Meta = (AllowPrivateAccess = "true"))
	mutable bool bOnGround = false;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FP Character Movement|Wall Running", Meta = (AllowPrivateAccess = "true"))
	float VaultRunSpeed = 600.0f;
	
	/** Vaulting Timer reset value*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FP Character Movement|Wall Running", Meta = (AllowPrivateAccess = "true"))
	float VaultTimerBase = 0.5f;
	
	/** Vaulting Timer*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FP Character Movement|Wall Running", Meta = (AllowPrivateAccess = "true"))
	float VaultTimer = VaultTimerBase;

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

public:
	/** Velocity */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "FP Character Movement", Meta = (AllowPrivateAccess = "true"))
	FVector CurrentVelocity = FVector(0.0f, 0.0f, 0.0f);

	/** Move Speed */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "FP Character Movement", Meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 0.0f;
#pragma endregion

	
#pragma region Custom Movement Functions
	/** Returns true if the required mantling keys are down */
	bool AreRequiredMantleKeysDown() const;
	
	/** Begins the player's mantle */
	UFUNCTION(BlueprintCallable, Category= "Custom Player Movement")
	void BeginMantle();

	/** Ends the player's mantle */
	UFUNCTION(BlueprintCallable, Category= "Custom Player Movement")
	void EndMantle();

	/**
	*Returns true if the player is next to a wall that can be vaulted
	* @param vertical_tolerance The tolerance of how vertical we have to be to do a vault or mantle
	*/
	UFUNCTION(BlueprintCallable, Category= "Custom Player Movement")
	bool IsNextToWall(float vertical_tolerance = 0.0f);
	
	/**
	*Returns true if the angle between the player and the wall is small enough to start climbing it
	*/
	UFUNCTION(BlueprintCallable, Category= "Custom Player Movement")
	bool CheckAngleToWall();
	
	/**
	*Returns true if the player is on the ground
	*/
	UFUNCTION(BlueprintCallable, Category= "Custom Player Movement")
	bool IsOnGround();
	
#pragma endregion
};
