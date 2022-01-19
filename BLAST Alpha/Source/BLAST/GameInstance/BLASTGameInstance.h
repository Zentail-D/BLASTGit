// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SoundEngine.h"
#include "Engine/GameInstance.h"
#include "BLASTGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLAST_API UBLASTGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(BlueprintReadWrite)
	ASoundEngine* SoundEngine;
	
};
