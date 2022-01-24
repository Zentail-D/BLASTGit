// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlastTestGameMode.h"
#include "BlastTestHUD.h"
#include "BlastTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ABlastTestGameMode::ABlastTestGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ABlastTestHUD::StaticClass();
}
