// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyControllerParent.generated.h"

/**
 * 
 */
UCLASS()
class BLAST_API AEnemyControllerParent : public AAIController
{
	GENERATED_BODY()

	public:
	/**
	*Contructor used to initialize the behavior tree and blackboard components. Calls SetupPerceptionSystem
	*/
	AEnemyControllerParent(FObjectInitializer const& object_initializer = FObjectInitializer::Get());


	/**
	 *On Possess run when a character is possess or when the ai controller starts affecting the character
	 *@param pawn The character that is getting possess so that we have access to it
	 */
	void OnPossess(APawn* const pawn) override;

	void BeginPlay() override;

	/**
	*Blackboard Component used by the BehaviorTree. This is the getter to access the blackboard associated with this character
	*/
	class UBlackboardComponent* GetBlackboard() const;

	/**
	* Called when the character senses another character and checks their affiliation to that character. Uses internal variables to find affiliation
	* @param OtherActor - Upon return, will find the Attitude of the character to this actor using Generic Team ID's
	* @warning Only call this if the GenericTeamID were set properly in blueprints for Friend, Enemies, and Neutrals    
	*/
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& other) const override;
	

	/**
	*Initialize the Friendlies Team ID for the GetTeamAttitudeTowards. Any Actor with this team id will be considered an ally to this character. Initialized to zero
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "AI")
	FGenericTeamId FriendliesTeamId = FGenericTeamId(0);

	/**
	*Initialize the Enemies Team ID for the GetTeamAttitudeTowards. Any Actor with this team id will be considered an enemy to this character. Uninitialized
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "AI")
	FGenericTeamId EnemiesTeamId;

	/**
	*Initialize the Neutrals Team ID for the GetTeamAttitudeTowards. Any Actor with this team id will be considered an neutral to this character. Initalized to one
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "AI")
	FGenericTeamId NeutralsTeamId = FGenericTeamId(1);

	/**
	*Initialize this Characters Team ID. This is what all the other actors in the level will see when attempting to figure out if it is friendly or hostile. Return value for GetGenericTeamId
	*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= "AI")
	FGenericTeamId GenericTeamId;

	/**
	 *Ran Every Frame and Execute any code in this function every frame.
	 *@param DeltaTime the amount of time that has passed since the last frame to use in calculations
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	*Loop through all of the actors in the world and check if you are sensing a new stimulus. Uses the Generic Team ID to determine if they see a target
	*@param updated_actors, the list of actors that you need to update and check if they see a target
	*/
	UFUNCTION()
	void on_updated(TArray<AActor*> const& updated_actors);
	/**
	*BehaviorTree associated with the character. This will be the BehaviorTree that this character runs
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	/**
	*Customizable Sense in the AI. This is what determines how far the ai and "See" and if it can see allies, hostiles, or neutrals.
	*SightRadius is how far the AI can see,
	*LoseSightRadius is how far out of range of the sight radius the character needs to be for the ai to completely lose the character and this should be a little larger than the sight radius,
	*PeripheralVisionAngleDegrees angle in front of the ai that it sees at. 90 would be a cone in front of the AI. Think of a unit circle around the character,
	*SetMaxAge how long the ai will "remember" the last stimulus it had,
	*AutoSuccessRangeFromLastSeenLocation how far from the previous sight location the character needs to move for it to do another check on if it see the player or not,
	*DetectionByAffiliation.bDetectEnemies does the AI "See" Enemies that are tagged by the TeamIDs,
	*DetectionByAffiliation.bDetectFriendlies does the AI "See" Friendlies that are tagged by the TeamIDs,
	*DetectionByAffiliation.bDetectNeutrals does the AI "See" Neutrals that are tagged by the TeamIDs
	*/
	class UAISenseConfig_Sight* SenseConfig_Sight;

	/**
	*Customizable Sense in the AI. This determines the hearing of the ai
	*HearingRange Distance the AI will hear the character from,
	*DetectionByAffiliation.bDetectEnemies does the AI "See" Enemies that are tagged by the TeamIDs,
	*DetectionByAffiliation.bDetectFriendlies does the AI "See" Friendlies that are tagged by the TeamIDs,
	*DetectionByAffiliation.bDetectNeutrals does the AI "See" Neutrals that are tagged by the TeamIDs
	*/
	class UAISenseConfig_Hearing* SenseConfig_Hearing;

	/**
	*BehaviorTree is initialized through this component
	*/
	class UBehaviorTreeComponent* BehaviorTreeComponent;
	

	/**
	*Initialize the perception stimulus component. Hearing Component is initialized and the sight is initialized along with it.
	*Override the parameters in the SenseConfig's in blueprints. 
	*/
	void SetupPerceptionSystem();

	/**
	 * Boolean that is states whether or not the previous stimulus is the same as the current stimulus. There has to be two false before it switches to false to ensure that the ai cant rapid flip the boolean true and false.
	 */
	bool bPrevStimulusChange =false;
	/**
	 *Inside clock used to make sure that the Stimulus change is necessary and were not switching variables to switch them back 
	 */
	float StimulusTimer= 0.5f;

	/**
	 * Stops the Behavior Tree and Stop perception component from updating. Stop the movement right where it is
	 */
	void FreezeBehavior();
	
	/**
	* Restarts the Behavior Tree and Perception component
	*/
	void UnFreezeBehavior();
private:
	/**
	 *Returns the GenericTeamID that will determine if a character is a friend, enemy or neutral character based off of GenericTeamID
	 *@return FGenericTeamId, it is the GenericTeamId that is used in GiveAttitudeTowards.
	 */
	FGenericTeamId GetGenericTeamId() const override;

	/**
	*Blackbaord that is associated with the character. It is returned in GetBlackboard
	*/
	class UBlackboardComponent* BlackboardComponent;
};
