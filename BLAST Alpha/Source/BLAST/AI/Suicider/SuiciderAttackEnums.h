
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SuiciderAttackEnums.generated.h"
/**
* Enumeration types that are used for the CurrentMovementState and PrevMovementState variables in the UPlayerMovement class
*/
UENUM(BlueprintType)
enum class ESuiciderAttackEnums: uint8
{
	/** Will have the same Implementation as red unless we want it to change */
	VE_None UMETA(DisplayName="None"),

	/** Green Color for the Suicider Behavior
	 * For Later Implementation if we want to make a variety of Suiciders
	 * Green Idea was to make the suicider multiply
	 */
	VE_Green UMETA(DisplayName="Green"),

	/** Red Color for the Suicider Behavior
	 *For Later Implementation if we want to make a variety of Suiciders
	 * Red Idea was to make it deal a large chunk of damage
	 */
	VE_Red	UMETA(DisplayName = "Red"),

	/** Blue Color For Suicider Behavior
	 * For Later Implementation if we want a variety of Suiciders
	 * Blue Ideas was to make it alert all the enemies and slow playeer
	 */
	VE_Blue	UMETA(DisplayName = "Blue")

};

