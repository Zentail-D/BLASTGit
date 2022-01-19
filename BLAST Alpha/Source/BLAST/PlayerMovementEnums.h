// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "PlayerMovementEnums.generated.h"
/**
* Enumeration types that are used for the CurrentMovementState and PrevMovementState variables in the UPlayerMovement class
*/
UENUM(BlueprintType)
enum class EMovementStates: uint8
{
	/** None (movement state is either idle, walking, or mid-air(falling)) */
     VE_None UMETA(DisplayName="None"),

	/** LedgeGrab (Player is grabbing the ledge) */
     VE_LedgeGrab UMETA(DisplayName="Ledge Grab"),

	/**
	 * Mantle (Player is climbing up from the ledge)
	 * @warning CurrentMovementState should only be this if the Player is mid-air or if
	 *			PrevMovementState is LedgeGrab. Player must also be holding forward.
	 */
     VE_Mantle UMETA(DisplayName="Mantle"),

	/**
	 * Slide (Player is sliding along the ground)
	 * @warning CurrentMovementState should only be this if PrevMovementState is Sprint and Player is both
	 *			holding forward and on the ground.
	 */
     VE_Slide UMETA(DisplayName="Slide"),
	
	/**
	* Sprint (Player is running along the ground)
	* @warning CurrentMovementState should only be this if PrevMovementState is None and Player is both
	*			holding forward and on the ground.
	*/
     VE_Sprint UMETA(DisplayName="Sprint"),

	/**
	* Crouch (Player is crouched)
	* @warning CurrentMovementState should only be this if PrevMovementState is None and Player is on the ground.
	*/
     VE_Crouch UMETA(DisplayName="Crouch"),

	/**
	 * VerticalWallRun (Player is climbing up a vertical surface)
	 * @warning CurrentMovementState should only be this if PrevMovementState is None and if the Player is both
	 *			holding forward and in mid-air
	 */
	 VE_VerticalWallRun	UMETA(DisplayName="Vertical Wall Run")
};


/**
* Enumeration types that are used for the CurrentMovementMode and PrevMovementMode variables in the UPlayerMovement class
*/
UENUM()
enum class EMovement: uint8
{
	/** None (movement is disabled). */
	MOVE_None		UMETA(DisplayName="None"),

	/** Walking on a surface. */
	MOVE_Walking	UMETA(DisplayName="Walking"),

	/** 
	* Simplified walking on navigation data (e.g. navmesh). 
	* If GetGenerateOverlapEvents() is true, then we will perform sweeps with each navmesh move.
	* If GetGenerateOverlapEvents() is false then movement is cheaper but characters can overlap other objects without some extra process to repel/resolve their collisions.
	*/
	MOVE_NavWalking	UMETA(DisplayName="Navmesh Walking"),

	/** Falling under the effects of gravity, such as after jumping or walking off the edge of a surface. */
	MOVE_Falling	UMETA(DisplayName="Falling"),

	/** Swimming through a fluid volume, under the effects of gravity and buoyancy. */
	MOVE_Swimming	UMETA(DisplayName="Swimming"),

	/** Flying, ignoring the effects of gravity. Affected by the current physics volume's fluid friction. */
	MOVE_Flying		UMETA(DisplayName="Flying"),

	/** User-defined custom movement mode, including many possible sub-modes. */
	MOVE_Custom		UMETA(DisplayName="Custom"),

	MOVE_MAX		UMETA(Hidden)
};