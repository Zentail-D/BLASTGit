#pragma once

#include "UObject/ObjectMacros.h"

/**Custom Status effect for the weapons for when we need to implement status effects */
UENUM(BlueprintType)
enum class EWeaponStatusEffects : uint8
{
	Ve_None				UMETA(DisplayName = "None"),
	Ve_Fire				UMETA(DisplayName = "Fire")
};

/**Enums for our inventory and the different mod slots */
UENUM(BlueprintType)
enum class EModSlots : uint8
{
	Ve_DEFAULT			UMETA(DisplayName = "Default"),
	Ve_SLOT1			UMETA(DisplayName = "Slot1"),
	Ve_SLOT2			UMETA(DisplayName = "Slot2")
};
