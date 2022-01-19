// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/EnemyControllerParent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "blackboard_keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


AEnemyControllerParent::AEnemyControllerParent(FObjectInitializer const& object_initializer)
{
	//Initialize the BehaviorTreeComponent
	BehaviorTreeComponent = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
	//Initialize the BlackboardComponent
	BlackboardComponent = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
	//Runs Perception setup which initalizes all the perception components
	SetupPerceptionSystem();
}

void AEnemyControllerParent::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	if(BehaviorTree)
	{
		if (BlackboardComponent)
		{
			BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		}
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
}

void AEnemyControllerParent::BeginPlay()
{
	Super::BeginPlay();
}

UBlackboardComponent* AEnemyControllerParent::GetBlackboard() const
{
	return BlackboardComponent;
}

ETeamAttitude::Type AEnemyControllerParent::GetTeamAttitudeTowards(const AActor& other) const
{
	if(const APawn* OtherPawn = Cast<APawn>(&other))
	{
		if(const IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			//Gets the TeamID
			FGenericTeamId OtherTeamID = TeamAgent->GetGenericTeamId();
			if(OtherTeamID == FriendliesTeamId)
			{
				return ETeamAttitude::Friendly;
			}
			else if(OtherTeamID == EnemiesTeamId)
			{
				return ETeamAttitude::Hostile;
			}
			else
			{
				return ETeamAttitude::Neutral;
			}
		}
		return ETeamAttitude::Hostile;
	}
	//If it isnt a pawn then we are neutral to it
	return ETeamAttitude::Neutral;
}

void AEnemyControllerParent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Checks to see if the sight has been lost for a extended period so that we switch it to false.
	//The AI will sometimes lose the player for a split second and change the can see to false and
	//then positive and that reset shot timers so we give them a delay so that it makes sure that it actually
	//lose the player
	if(bPrevStimulusChange==false)
	{
		StimulusTimer-=DeltaTime;
		if(StimulusTimer<=0)
		{
			//If we cant see the player for a extended time then we will change the seeing of the player to false
			bPrevStimulusChange=true;
			StimulusTimer=0.5;
			GetBlackboard()->SetValueAsBool(bb_keys::can_see_target,false);
		}
	}
	else
	{
		StimulusTimer=0.5;
	}
}

void AEnemyControllerParent::on_updated(TArray<AActor*> const& updated_actors)
{
	//Loop through all the actors
	for (size_t x = 0; x < updated_actors.Num(); ++x)
	{
		//Gets the perception info on a specific actor from a reference to that actor and saved into info
		FActorPerceptionBlueprintInfo ActorPerceptionInfo;
		GetPerceptionComponent()->GetActorsPerception(updated_actors[x], ActorPerceptionInfo);
		for (size_t k = 0; k < ActorPerceptionInfo.LastSensedStimuli.Num(); ++k)
		{
			FAIStimulus const Stimulus = ActorPerceptionInfo.LastSensedStimuli[k];
			if (Stimulus.Tag == tags::noise_tag)
			{
				//Set that the charcter needs to investigate and the location
				GetBlackboard()->SetValueAsBool(bb_keys::is_investigating, Stimulus.WasSuccessfullySensed());
				GetBlackboard()->SetValueAsVector(bb_keys::target_location, Stimulus.StimulusLocation);
			}
			else if (Stimulus.Type.Name == "Default__AISense_Sight")
			{
				//If the Previous stimulus was false twice in a row before it had a chance to reset to true through tick it will be false. Safe guard the rapid changing of true and false
				if(bPrevStimulusChange==false && Stimulus.WasSuccessfullySensed()==false)
				{
					GetBlackboard()->SetValueAsBool(bb_keys::can_see_target, false);
				}
				//IF the Stimulus is true we always set it becuase true will not switch all the time
				else if(Stimulus.WasSuccessfullySensed()==true)
				{
					GetBlackboard()->SetValueAsBool(bb_keys::can_see_target, true);
				}
				//Always set previous stimulus to whatever was last sensed
				bPrevStimulusChange=Stimulus.WasSuccessfullySensed();
				//Set the location to where the stimulus was last seen
				GetBlackboard()->SetValueAsVector(bb_keys::target_location, Stimulus.StimulusLocation);
			}
		}
	}
}

void AEnemyControllerParent::SetupPerceptionSystem()
{
	// create and initialise sight configuration object
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SenseConfig_Sight)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
		SenseConfig_Sight->SightRadius = 500.0f;
		SenseConfig_Sight->LoseSightRadius = SenseConfig_Sight->SightRadius + 25.0f;
		SenseConfig_Sight->PeripheralVisionAngleDegrees = 90.0f;
		SenseConfig_Sight->SetMaxAge(5.0f);
		SenseConfig_Sight->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		SenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
		SenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
		SenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;

		// add sight configuration component to perception component
		GetPerceptionComponent()->SetDominantSense(*SenseConfig_Sight->GetSenseImplementation());
		GetPerceptionComponent()->ConfigureSense(*SenseConfig_Sight);
	}

	// create and initialise hearing config object
	SenseConfig_Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing config"));
	if (SenseConfig_Hearing)
	{
		SenseConfig_Hearing->HearingRange = 3000.0f;
		SenseConfig_Hearing->DetectionByAffiliation.bDetectEnemies = true;
		SenseConfig_Hearing->DetectionByAffiliation.bDetectFriendlies = false;
		SenseConfig_Hearing->DetectionByAffiliation.bDetectNeutrals = true;

		// add sight configuration component to perception component
		GetPerceptionComponent()->ConfigureSense(*SenseConfig_Hearing);
	}
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyControllerParent::on_updated);
}

void AEnemyControllerParent::FreezeBehavior()
{
	StopMovement();
	//BehaviorTreeComponent->StopTree();
	BehaviorTreeComponent->PauseLogic(FString("Freezing Behavior"));
	GetPerceptionComponent()->Deactivate();
	//GetPerceptionComponent()->SetActive(false);
}

void AEnemyControllerParent::UnFreezeBehavior()
{
	BehaviorTreeComponent->ResumeLogic(FString("Unfreezing Logic"));
	GetPerceptionComponent()->Activate();
	//GetPerceptionComponent()->SetActive(true);
}

FGenericTeamId AEnemyControllerParent::GetGenericTeamId() const
{
	return GenericTeamId;
}
