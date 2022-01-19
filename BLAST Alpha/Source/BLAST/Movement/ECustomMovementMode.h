#pragma once

#include "UObject/ObjectMacros.h"

/** Custom movement modes for Characters. */
UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_WallRunning   UMETA(DisplayName = "WallRunning"),
	CMOVE_Slide			UMETA(DisplayName = "Slide"),
	CMOVE_Mantle		UMETA(DisplayName = "Mantle"),
	CMOVE_Vaulting		UMETA(DisplayName = "Vaulting"),
	CMOVE_MAX			UMETA(Hidden),
};
