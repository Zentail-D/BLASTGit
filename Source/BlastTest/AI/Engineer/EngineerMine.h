// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineerToolsParent.h"

#include "EngineerMine.generated.h"

/**
 * 
 */
UCLASS()
class  AEngineerMine : public AEngineerToolsParent
{
	GENERATED_BODY()
public:

	/**
	* Enable the tick for the drone swarm
	*/
	AEngineerMine();

	/**
	 *Called every frame
	 * Checks to see if it needs to be destroy and will Destroy it unless it bNeverDestroy is true
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	* Overriding function to call when a tool needs to be destroyed
	*/
	void SafeToolDestroy() override;

	/**
	 * Used Queuing it destroy pattern
	 */
	bool bNeedDestroyed = false;

	/**
	* Used to start the destroy timer
	*/
	bool bStartDestroyTimer = false;

	
protected:
	/**
	 * Sets the mine to never destroy itself
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Mine Settings", meta = (AllowProtectedAccess = "true"))
	bool bNeverDestroy=false;

	/**
	* Sets the mine to destroy itself after the engineer dies
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Mine Settings", meta = (AllowProtectedAccess = "true"))
	bool bDestroyItselfOnEngineerDeath=true;
	
	/**
	* Sets the life time the mine has when set to destroy itself
	* If the boolean bDestroyItselfOnEngineerDeath is true this will get be how long the mine survives after engineer dies
	* If false this is how long a mine stays alive for even if engineer is alive
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings | Mine Settings", meta = (AllowProtectedAccess = "true"))
	float MineLifeTime= 5;

	private:
	/**
	* Inform the engineer that it has died
	*/
	void DestroyChildren() override;
};
