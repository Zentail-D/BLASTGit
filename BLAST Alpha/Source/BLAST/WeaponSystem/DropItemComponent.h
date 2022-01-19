// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DropItemComponent.generated.h"
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLAST_API UDropItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDropItemComponent();

	/**
	 * Used to ensure that the ModOptions array is empty on destruction
	 */
	virtual ~UDropItemComponent() override;

	/**
	 * @param SpawnLocation Location that the mod that is randomly chosen from the ModOptions array will be spawned in at
	 * Spawns in a Mod which can be a generator point or ModParent
	*/
	void DropRandomMod(FVector SpawnLocation);
	
protected:
	/**
	 * Array that will hold all the options for the ai to spawn in
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Drops", meta = (AllowProtectedAccess = "true"))
	TArray<TSubclassOf<class AModParent>> ModOptions;
};
