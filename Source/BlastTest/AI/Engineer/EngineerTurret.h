// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineerToolsParent.h"
#include "EngineerTurret.generated.h"

/**
 * 
 */
UCLASS()
class  AEngineerTurret : public AEngineerToolsParent
{
	GENERATED_BODY()

public:
	/**
	* Enable the tick for the Turret
	*/
	AEngineerTurret();

	/**
	 * @return Getter for Degree Rotation
	 */
	float GetDegreeRotation();

	/**
	 * @return Getter for TurretPatrolRotationRate
	 */
	float GetTurretPatrolRotationRate();

	/**
	 * @return Getter for TurretPlayerFoundRotationRate
	 */
	float GetTurretPlayerFoundRotationRate();

	/**
	 * @return Getter for StartingRotation
	 */
	FRotator GetStartingRotation();

	/**
	* Used to start the destroy timer
	*/
	bool bStartDestroyTimer = false;

	virtual void Tick(float DeltaSeconds) override;

	/**
	* Used Queuing it destroy pattern
	*/
	bool bNeedDestroyed = false;

	/**
	* Overriding function to call when a tool needs to be destroyed
	*/
	void SafeToolDestroy() override;
	

protected:
	
	/**The degrees of free motion for the turret. This is how far the turret will rotate each direction from the starting rotation*/
	UPROPERTY(EditAnywhere,Category="AI Settings")
	float DegreeRotation;

	/**
	 * Rate at which the turret will rotate at. The speed at which it rotates at while patrolling
	 */
	UPROPERTY(EditAnywhere,Category="AI Settings")
	float TurretPatrolRotationRate;

	/**
	 * Rate at which the turret will rotate at. The speed at which it rotates at while targeting the players
	 */
	UPROPERTY(EditAnywhere,Category="AI Settings")
	float TurretPlayerFoundRotationRate;

	/**Rotation to save what we started at. Whatever the rotation is at when the BeginPlay is trigger will be set as starting rotation*/
	FRotator StartRotation;

	/**
	 *Saves off the starting rotation so that it will always go back to the rotation that it held originally
	*Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;
	
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
	float TurretLifeTime= 5;

	private:
	/**
	* Inform the engineer that it has died
	*/
	void DestroyChildren() override;
};
