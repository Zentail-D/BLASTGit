// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShake.h"
#include "NetworkCharCameraShake.generated.h"

/**
 * Shakes the camera depending on what the player's movement is.
 */
UCLASS()
class BLAST_API UNetworkCharCameraShake : public UMatineeCameraShake
{
	GENERATED_BODY()

public:
	/** Constructor for the class UNetworkCameraShake */
	UNetworkCharCameraShake();
	
};
