// Fill out your copyright notice in the Description page of Project Settings.

#include "Movement/NetworkCharCameraShake.h"


UNetworkCharCameraShake::UNetworkCharCameraShake()
{
	//Changing the default values of how long the shake lasts and blends in and out
	OscillationDuration = 1.0f;
	OscillationBlendInTime = 0.5f;
	OscillationBlendOutTime = 0.5f;

	//Shake rotation of the Y-axis
	RotOscillation.Roll.Amplitude = 1.0f;
	RotOscillation.Roll.Frequency = 2.0f;
}
