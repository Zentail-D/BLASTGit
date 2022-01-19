// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkCharMC.h"

#include <string>

#include "NetworkChar.h"
#include "GameFramework/Character.h"
#include "ECustomMovementMode.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

FNetworkPredictionData_Client* UNetworkCharMC::GetPredictionData_Client() const
{
	if (ClientPredictionData == nullptr)
	{
		// Return our custom client prediction class instead
		UNetworkCharMC* MutableThis = const_cast<UNetworkCharMC*>(this);
		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_My(*this);
	}

	return ClientPredictionData;
}

void UNetworkCharMC::SetSprinting(bool sprinting)
{
	SprintKeyDown = sprinting;
}

bool UNetworkCharMC::ReturnCrouch() const
{
	return bCrouch;
}

// void UNetworkCharMC::SetCrouch(bool set)
// {
// 	bCrouch = set;
// }

bool UNetworkCharMC::ReturnMoveLock() const
{
	return bLockMovement;
}

FVector UNetworkCharMC::GetSlideVelocity() const
{
	if(WantsToSlide)
	{
		
		return newVelocity;
	}
	else
	{
		//if(GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("This is wrong!"));
		return newVelocity;
	}
}

void UNetworkCharMC::SetAirControl(int a)
{
	AirControl = a;
}

void UNetworkCharMC::CameraTick() const
{
	switch (CustomMovementMode)
	{
	case CMOVE_Slide:
		{
			CameraTilt(-15);
			break;
		}
	default:
		{
			CameraTilt(0);
		}
	}
}

void UNetworkCharMC::CameraTilt(float TargetRoll) const
{
	ANetworkChar* temp_camera = Cast<ANetworkChar>(GetOwner());
	
}
	


bool UNetworkCharMC::BeginVaultingRun()
{
	// Only allow wall running to begin if the required keys are down
	if ((VaultingKeysDown == true) && (CheckAngle == true) && (IsFalling()) && (!ReturnCrouch()))
	{
		// Set the movement mode to wall running. UE4 will handle replicating this change to all connected clients.
		SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_Vaulting);
		return true;
	}

	return false;
}

void UNetworkCharMC::EndVaultingRun()
{
	// Set the movement mode back to falling
	SetMovementMode(EMovementMode::MOVE_Falling);
	GravityScale = 2.5;
}

bool UNetworkCharMC::AreRequiredVaultingRunKeysDown() const
{
	// If timer is already depleted, return false regardless if the keys are down
	if (VaultTimer < 0.0f)
	{
		return false;
	}
	
	// Since this function is checking for input, it should only be called for locally controlled character
	if (GetPawnOwner()->IsLocallyControlled() == false)
		return false;

	// Make sure the spring key is down (the player may only wall run if he's hold sprint)
	TArray<FInputActionKeyMapping> sprintKeyMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName("Sprint", sprintKeyMappings);
	for (FInputActionKeyMapping& sprintKeyMapping : sprintKeyMappings)
	{
		
		if (GetPawnOwner()->GetController<APlayerController>()->IsInputKeyDown(sprintKeyMapping.Key))
		{
			return true;
		}
		
	}
	return true;
	//return false;
}

// \/ We possibly don't need this code for rebuilding \/
bool UNetworkCharMC::AreRequiredMantleRunKeysDown() const
{
	// If timer is already depleted, return false regardless if the keys are down
	if (VaultTimer < 0.0f)
	{
		return false;
	}
	
	// Since this function is checking for input, it should only be called for locally controlled character
	if (GetPawnOwner()->IsLocallyControlled() == false)
		return false;

	// Make sure the spring key is down (the player may only wall run if he's hold sprint)
	TArray<FInputActionKeyMapping> sprintKeyMappings;
	UInputSettings::GetInputSettings()->GetActionMappingByName("Sprint", sprintKeyMappings);
	for (FInputActionKeyMapping& sprintKeyMapping : sprintKeyMappings)
	{
		
		if (GetPawnOwner()->GetController<APlayerController>()->IsInputKeyDown(sprintKeyMapping.Key))
		{
			return true;
		}
		
	}
	return true;
	//return false;
}

bool UNetworkCharMC::BeginMantle()
{
	if ((MantleKeysDown == true) && (CheckAngle == true) && (IsFalling()) && (!ReturnCrouch()))
	{
		// Set the movement mode to wall running. UE4 will handle replicating this change to all connected clients.
		SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_Mantle);
		return true;
	}

	return false;
}

void UNetworkCharMC::EndMantle()
{
	// Set the movement mode back to falling
	SetMovementMode(EMovementMode::MOVE_Falling);
	GravityScale = 2.5;
}
// ^ We possibly don't need this code for rebuilding ^

bool UNetworkCharMC::AreRequiredSlideRunKeysDown() const
{
	// Since this function is checking for input, it should only be called for locally controlled character
	if (GetPawnOwner()->IsLocallyControlled() == false)
		return false;

	if (!bOnGround)
	{
		return false;
	}

	if (bIsSlide)
	{
		return true;
	}
	
	if ((!IsFalling()) && (Velocity.Size() > 900.0f) && (bCrouch))
	{
		return true;			
	}
	return false;
}

void UNetworkCharMC::BeginSlide()
{
	if (WantsToSlide == true)
	{
		// Set the movement mode to wall running. UE4 will handle replicating this change to all connected clients.
		//if(GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Green, TEXT("We Ready are sliding!"));
		SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::CMOVE_Slide);
	}
}



void UNetworkCharMC::EndSlide()
{
	SlideSpeed = BaseSlideSpeed;
	bIsSlide = false;
	GroundFriction = 8.0f;
	BrakingDecelerationWalking = 500.f;
	MaxWalkSpeedCrouched = 300.0f;
	SetPlaneConstraintEnabled(false);
	CameraTilt(0.0f);
	// Set the movement mode back to falling
	SetMovementMode(EMovementMode::MOVE_NavWalking);
}


void UNetworkCharMC::BeginWalkCameraShake()
{
	GetWorld()->GetFirstLocalPlayerFromController()->PlayerController->ClientStartCameraShake(NetworkCameraShake, 1.0f);
}


void UNetworkCharMC::EndWalkCameraShake()
{
	GetWorld()->GetFirstLocalPlayerFromController()->PlayerController->ClientStopCameraShake(NetworkCameraShake);
}


bool UNetworkCharMC::IsNextToWall(float vertical_tolerance)
{
	// Do a line trace from the player into the wall to make sure we're stil along the side of a wall
	FVector crossVector = WallRunSide == EWallRunSide::kLeft ? FVector(0.0f, 0.0f, -1.0f) : FVector(0.0f, 0.0f, 1.0f);
	FVector traceStart = GetPawnOwner()->GetActorLocation() + (WallRunDirection * 20.0f);
	FVector traceEnd = traceStart + (FVector::CrossProduct(WallRunDirection, crossVector) * 50.0f);
	FHitResult hitResult;

	// Create a helper lambda for performing the line trace
	auto lineTrace = [&](const FVector& start, const FVector& end)
	{
		return (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility));
	};
	
	// If a vertical tolerance was provided we want to do two line traces - one above and one below the calculated line
	if (vertical_tolerance > FLT_EPSILON)
	{
		// If both line traces miss the wall then return false, we're not next to a wall
		if (lineTrace(FVector(traceStart.X, traceStart.Y, traceStart.Z + vertical_tolerance / 2.0f), FVector(traceEnd.X, traceEnd.Y, traceEnd.Z + vertical_tolerance / 2.0f)) == false &&
			lineTrace(FVector(traceStart.X, traceStart.Y, traceStart.Z - vertical_tolerance / 2.0f), FVector(traceEnd.X, traceEnd.Y, traceEnd.Z - vertical_tolerance / 2.0f)) == false)
		{
			return false;
		}
	}
	// If no vertical tolerance was provided we just want to do one line trace using the caclulated line
	else
	{
		// return false if the line trace misses the wall
		if (lineTrace(traceStart, traceEnd) == false)
			return false;
	}

	if ((hitResult.Component->ComponentHasTag("projectiles")) || (hitResult.Component->ComponentHasTag("Unclimbable")))
	{
		return false;
	}

	// Make sure we're still on the side of the wall we expect to be on
	EWallRunSide newWallRunSide;
	FindVaultingDirectionAndSide(hitResult.ImpactNormal, WallRunDirection, newWallRunSide);
	if (newWallRunSide != WallRunSide)
	{
		return false;
	}

	WallNormal = hitResult.Normal;
	WallNormal.Normalize();
	
	CheckAngle = CheckAngleToWall();
	return true;
}

bool UNetworkCharMC::CheckAngleToWall()
{
	ANetworkChar* temp_vault = Cast<ANetworkChar>(GetOwner());
	FVector temp_forward;
	if(temp_vault)
	{
		temp_forward = temp_vault->GetActorForwardVector();
		temp_forward.Normalize();
	}
	float temp_angle = acosf(FVector::DotProduct(temp_forward, WallNormal));

	if (temp_angle < 2.5f)
	{
		return false;
	}
	return true;
}

bool UNetworkCharMC::IsOnGround()
{
	// Start the raycast looking toward the ground
	FVector traceStart = GetPawnOwner()->GetActorLocation();
	FVector traceEnd = FVector(traceStart.X, traceStart.Y, traceStart.Z - 500.0f);
	FHitResult hitResult;

	// Create a helper lambda for performing the line trace
	auto lineTrace = [&](const FVector& start, const FVector& end)
	{
		return (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility));
	};
	
	// If raycast hits something, return true, else, return false
	if (lineTrace(traceStart, traceEnd) == false)
	{
		return false;
	}
	groundNormal = hitResult.Normal;
	groundNormal.Normalize();
	return true;
}

void UNetworkCharMC::FindVaultingDirectionAndSide(const FVector& surface_normal, FVector& direction, EWallRunSide& side) const
{
	FVector crossVector;

	if (FVector2D::DotProduct(FVector2D(surface_normal), FVector2D(GetPawnOwner()->GetActorRightVector())) > 0.0)
	{
		side = EWallRunSide::kRight;
		crossVector = FVector(0.0f, 0.0f, 1.0f);
	}
	else
	{
		side = EWallRunSide::kLeft;
		crossVector = FVector(0.0f, 0.0f, -1.0f);
	}

	// Find the direction parallel to the wall in the direction the player is moving
	direction = FVector::CrossProduct(surface_normal, crossVector);
}

bool UNetworkCharMC::CanSurfaceBeVaulting(const FVector& surface_normal) const
{
	// Return false if the surface normal is facing down
	if (surface_normal.Z < -0.05f)
		return false;

	FVector normalNoZ = FVector(surface_normal.X, surface_normal.Y, 0.0f);
	normalNoZ.Normalize();

	// Find the angle of the wall
	float wallAngle = FMath::Acos(FVector::DotProduct(normalNoZ, surface_normal));

	// Return true if the wall angle is less than the walkable floor angle
	return wallAngle < GetWalkableFloorAngle();
}

bool UNetworkCharMC::IsCustomMovementMode(uint8 custom_movement_mode) const
{
	return MovementMode == EMovementMode::MOVE_Custom && CustomMovementMode == custom_movement_mode;
}

void UNetworkCharMC::OnActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsCustomMovementMode(ECustomMovementMode::CMOVE_WallRunning))
		return;

	if (IsCustomMovementMode(ECustomMovementMode::CMOVE_Vaulting))
		return;
	
	if (IsCustomMovementMode(ECustomMovementMode::CMOVE_Mantle))
		return;
	
	if(IsCustomMovementMode(ECustomMovementMode::CMOVE_Slide))
		return;
	// Make sure the surface can be wall ran based on the angle of the surface that we hit
	if (CanSurfaceBeVaulting(Hit.ImpactNormal) == false)
		return;

	// Update the wall run direction and side
	FindVaultingDirectionAndSide(Hit.ImpactNormal, WallRunDirection, WallRunSide);

	// Make sure we're next to a wall
	if (IsNextToWall() == false)
		return;
	
	if(IsFalling())
		BeginVaultingRun();
	else
	{
		// if(!OtherActor->ActorHasTag("Pickup"))
		// {
		BeginMantle();
		// }
		
	}
		
}

void UNetworkCharMC::BeginPlay()
{
	Super::BeginPlay();

	// We don't want simulated proxies detecting their own collision
	if (GetPawnOwner()->GetLocalRole() > ROLE_SimulatedProxy)
	{
		// Bind to the OnActorHot component so we're notified when the owning actor hits something (like a wall)
		GetPawnOwner()->OnActorHit.AddDynamic(this, &UNetworkCharMC::OnActorHit);
	}
	SetAirControl(1);
	GroundFriction = 8.0f;
	BrakingDecelerationWalking = 500.f;
	MaxWalkSpeedCrouched = 300.0f;
}

void UNetworkCharMC::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (GetPawnOwner() != nullptr && GetPawnOwner()->GetLocalRole() > ROLE_SimulatedProxy)
	{
		// Unbind from all events
		GetPawnOwner()->OnActorHit.RemoveDynamic(this, &UNetworkCharMC::OnActorHit);
	}

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

// void UNetworkCharMC::Crouch(bool bClientSimulation)
// {
// 	Super::Crouch(bClientSimulation);
// 	if(GEngine)
// 		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("We are crouching!")); 
// 	ANetworkChar* temp_crouch = Cast<ANetworkChar>(GetOwner());
// 	if(temp_crouch)
// 	{
// 		temp_crouch->SetCamera(FVector(0, 0, 20));
// 	}
// 	bCrouch = true;
// 	
// }
// 
// void UNetworkCharMC::UnCrouch(bool bClientSimulation)
// {
// 	Super::UnCrouch(bClientSimulation);
// 	if(GEngine)
// 		GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("We are uncrouching!"));
// 	ANetworkChar* temp_uncrouch = Cast<ANetworkChar>(GetOwner());
// 	if(temp_uncrouch)
// 	{
// 		temp_uncrouch->SetCamera(FVector(0, 0, 40));
// 	}
// 	bCrouch = false;
// }

void UNetworkCharMC::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// Peform local only checks
	if (GetPawnOwner()->IsLocallyControlled())
	{
		if (SprintKeyDown == true)
		{
			
			
				// Only set WantsToSprint to true if the player is moving forward (so that he can't sprint backwards)
				FVector velocity2D = GetPawnOwner()->GetVelocity();
				FVector forward2D = GetPawnOwner()->GetActorForwardVector();
				velocity2D.Z = 0.0f;
				forward2D.Z = 0.0f;
				velocity2D.Normalize();
				forward2D.Normalize();
				//if(GEngine)
				//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("We are sprinting!"));
				WantsToSprint = FVector::DotProduct(velocity2D, forward2D) > 0.5f;
		}
		else
		{
			WantsToSprint = false;
		}

		// Update if the required wall run key(s) are being pressed
		VaultingKeysDown = AreRequiredVaultingRunKeysDown();
		WantsToSlide = AreRequiredSlideRunKeysDown();
		MantleKeysDown = AreRequiredMantleRunKeysDown();
		bOnGround = IsOnGround();
		if(WantsToSlide)
		{
			BeginSlide();
			//SlideTimer -= DeltaTime;
		}
		CameraTick();
		
	}
	

	//Head bobbing, needs fixed rotates when standing still.
	/*if(GetOwner()->GetVelocity().X > 1.0f || GetOwner()->GetVelocity().X < -1.0f
		|| GetOwner()->GetVelocity().Y > 1.0f || GetOwner()->GetVelocity().Y < -1.0f)
	{
		BeginWalkCameraShake();
	}
	else
	{
		EndWalkCameraShake();
	}*/
	
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Silver,
	// 		FString::Printf(TEXT("Velocity = %f "), Velocity.Size()));
	// }

	// Reset VaultTimer
	if (IsOnGround() && !(IsCustomMovementMode(CMOVE_Mantle)) && !(IsCustomMovementMode(CMOVE_Vaulting)))
	{
		VaultTimer = VaultTimerBase;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UNetworkCharMC::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	/*  There are 4 custom move flags for us to use. Below is what each is currently being used for:
		FLAG_Custom_0		= 0x10, // Sprinting
		FLAG_Custom_1		= 0x20, // Vaulting
		FLAG_Custom_2		= 0x40, // Sliding
		FLAG_Custom_3		= 0x80, // Mantle
	*/

	// Read the values from the compressed flags
	WantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	VaultingKeysDown = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
	WantsToSlide = (Flags & FSavedMove_Character::FLAG_Custom_2) != 0;
	MantleKeysDown = (Flags & FSavedMove_Character::FLAG_Custom_3) != 0;
}

void UNetworkCharMC::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	if (MovementMode == MOVE_Custom)
	{
		switch (CustomMovementMode)
		{
		// Did we just start wall running?
		case ECustomMovementMode::CMOVE_Vaulting:
		{
			// Stop current movement and constrain the character to only horizontal movement
			StopMovementImmediately();
			bConstrainToPlane = true;
			SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));
				break;
		}
		
		case ECustomMovementMode::CMOVE_Mantle:
			{
				// Stop current movement and constrain the character to only horizontal movement
				StopMovementImmediately();
				bConstrainToPlane = true;
				SetPlaneConstraintNormal(FVector(0.0f, 0.0f, 1.0f));
				break;
			}
		
		}
	}

	if (PreviousMovementMode == MOVE_Custom)
	{
		switch (PreviousCustomMode)
		{
		// Did we just finish wall running?
		case ECustomMovementMode::CMOVE_Vaulting:
		{
			// Unconstrain the character from horizontal movement
			bConstrainToPlane = false;
		}
		break;
		}
	}

	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

void UNetworkCharMC::PhysCustom(float deltaTime, int32 Iterations)
{
	// Phys* functions should only run for characters with ROLE_Authority or ROLE_AutonomousProxy. However, Unreal calls PhysCustom in
	// two seperate locations, one of which doesn't check the role, so we must check it here to prevent this code from running on simulated proxies.
	
	if (GetOwner()->GetLocalRole() == ROLE_SimulatedProxy)
		return;
	
	switch (CustomMovementMode)
	{
		case ECustomMovementMode::CMOVE_Vaulting:
		{
			PhysVaulting(deltaTime, Iterations);
			break;
		}
		case ECustomMovementMode::CMOVE_Slide:
		{
			PhysSliding(deltaTime, Iterations);
			break;
		}
		case ECustomMovementMode::CMOVE_Mantle:
		{
			PhysMantle(deltaTime, Iterations);
			break;
		}
	}

	// Not sure if this is needed
	Super::PhysCustom(deltaTime, Iterations);
}

void UNetworkCharMC::PhysVaulting(float deltaTime, int32 Iterations)
{
	// IMPORTANT NOTE: This function (and all other Phys* functions) will be called on characters with ROLE_Authority and ROLE_AutonomousProxy
	// but not ROLE_SimulatedProxy. All movement should be performed in this function so that is runs locally and on the server. UE4 will handle
	// replicating the final position, velocity, etc.. to the other simulated proxies.

	
	/*if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,
			FString::Printf(TEXT("Angle between player & wall = %f"), temp_angle));
	}*/
	if(!CheckAngle)
	{
		EndVaultingRun();
		return;
	}
	
	// Handle Vaulting Timer
	if (VaultTimer >= 0.0f)
	{
		VaultTimer -= deltaTime;
	}
	else if (VaultTimer < 0.0f)
	{
		EndVaultingRun();
	}

	// Make sure the required wall run keys are still down
	
	if (VaultingKeysDown == false)
	{
		//if(GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("We are no longer vaulting!"));
		EndVaultingRun();
		return;
	}
	

	// Make sure we're still next to a wall. Provide a vertial tolerance for the line trace since it's possible the the server has
	// moved our character slightly since we've began the wall run. In the event we're right at the top/bottom of a wall we need this
	// tolerance value so we don't immiedetly fall of the wall 
	if (IsNextToWall(LineTraceVerticalTolerance) == false)
	{
		EndVaultingRun();
		//if(GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, TEXT("We are no longer vaulting!"));
		return;
	}
	
	newVelocity = FVector(0.0f, 0.0f, 1.0f);
	newVelocity.X *= 1.0f;
    newVelocity.Y *= 1.0f;
    newVelocity.Z *= VaultRunSpeed;
	Velocity = newVelocity;
	
	if(newVelocity.Z >= 100.f)
	{
		EndVaultingRun();
		return;
	}
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);
	
}

// \/ We'll possibly replace/discard this code entirely while rebuilding movement \/
FVector UNetworkCharMC::SlideVector()
{
	ANetworkChar* temp_slide = Cast<ANetworkChar>(GetOwner());
	if(temp_slide)
	{
		FVector PlayerLocation = temp_slide->GetActorLocation();
		FVector Start, End;
		Start = PlayerLocation;
		End = temp_slide->GetActorUpVector()*-200.0f + PlayerLocation;
		FHitResult OutHit;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(),Start,End,ETraceTypeQuery::TraceTypeQuery1,
			true,TArray<AActor*>(),EDrawDebugTrace::None,OutHit,
			true,FLinearColor::Green,FLinearColor::Red,5.0f);
		FVector SlidingVector =FVector::CrossProduct( OutHit.ImpactNormal ,temp_slide->GetActorRightVector())*-1;
		return SlidingVector;
	}
	return FVector(0,0,0);
}

void UNetworkCharMC::PhysSliding(float deltaTime, int32 Iterations)
{
	ANetworkChar* temp_slide = Cast<ANetworkChar>(GetOwner());
	slideForward = temp_slide->GetActorRightVector();
	if (!bIsSlide)
	{
		
		if(temp_slide)
		{
			slideForward.Normalize();
			slideForward = FVector::CrossProduct(slideForward, groundNormal);
			bIsSlide = true;
		}
	}

	// get the angle between the ground and the forward vector
	float temp_angle = acosf(FVector::DotProduct(slideForward, groundNormal));

	// update forward vector to be parallel to the ground
	slideForward = FVector::CrossProduct(slideForward, groundNormal);

	if (IsFalling() || (!bOnGround) || (IsNextToWall()))
	{
		EndSlide();
	}

	newVelocity = slideForward;
	newVelocity *= SlideSpeed;

	if ((slideForward.Z >= -0.15f) && (slideForward.Z <= 0.15f))
	{
		SlideSpeed -= SlideFlatDeceleration * deltaTime;
	}
	else if (slideForward.Z > 0.15f)
	{
		SlideSpeed -= SlideUphillDeceleration * deltaTime;
	}
	else if (slideForward.Z < -0.15f)
	{
		SlideSpeed += SlideDownhillAcceleration * deltaTime;
	}

	Velocity = newVelocity;
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);

	if(Velocity.Size() < 35.0f)
	{
		EndSlide();
	}
}

void UNetworkCharMC::PhysMantle(float deltaTime, int32 Iterations)
{
	// IMPORTANT NOTE: This function (and all other Phys* functions) will be called on characters with ROLE_Authority and ROLE_AutonomousProxy
	// but not ROLE_SimulatedProxy. All movement should be performed in this function so that is runs locally and on the server. UE4 will handle
	// replicating the final position, velocity, etc.. to the other simulated proxies.

	// Make sure the required wall run keys are still down
	
	if ((MantleKeysDown == false) || (CheckAngle == false))
	{
		//if(GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("We are no longer Mantling!"));
		EndVaultingRun();
		return;
	}
	

	// Make sure we're still next to a wall. Provide a vertial tolerance for the line trace since it's possible the the server has
	// moved our character slightly since we've began the wall run. In the event we're right at the top/bottom of a wall we need this
	// tolerance value so we don't immiedetly fall of the wall 
	if (IsNextToWall(LineTraceVerticalTolerance) == false)
	{
		EndVaultingRun();
		//if(GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, TEXT("We are no Mantling!"));
		return;
	}

	// Set the owning player's new velocity based on the wall run direction
	newVelocity = FVector(0,0,1.0);
	newVelocity.X *= 1.0;
	newVelocity.Y *= 1.0;
	newVelocity.Z *= VaultRunSpeed;
	Velocity = newVelocity;
	if(newVelocity.Z >= 100.f)
	{
		EndVaultingRun();
		//if(GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Purple, TEXT("We are no Mantling!"));
		return;
	}

	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);
	SafeMoveUpdatedComponent(Adjusted, UpdatedComponent->GetComponentQuat(), true, Hit);
	
}
// ^ We'll possibly replace/discard this code entirely while rebuilding movement ^

float UNetworkCharMC::GetMaxSpeed() const
{
	switch (MovementMode)
	{
	case MOVE_Walking:
	case MOVE_NavWalking:
	{
		if (IsCrouching())
		{
			return MaxWalkSpeedCrouched;
		}
		if (WantsToSprint && !bCrouch)
		{
			return SprintSpeed;
		}

		return RunSpeed;
	}
	case MOVE_Falling:
		return RunSpeed;
	case MOVE_Swimming:
		return MaxSwimSpeed;
	case MOVE_Flying:
		return MaxFlySpeed;
	
	case MOVE_Custom:
		if (!bIsSlide)
			return MaxCustomMovementSpeed;
		else
		{
			return SlideSpeed;
		}
	
	case MOVE_None:
	default:
		return 0.f;
	}
}

float UNetworkCharMC::GetMaxAcceleration() const
{
	if (IsMovingOnGround())
	{
		
		if (WantsToSlide)
		{
			return Super::GetMaxAcceleration();
		}
		
		if (WantsToSprint)
		{
			return SprintAcceleration;
		}

		return RunAcceleration;
	}

	return Super::GetMaxAcceleration();
}

void UNetworkCharMC::ProcessLanded(const FHitResult& Hit, float remainingTime, int32 Iterations)
{
	Super::ProcessLanded(Hit, remainingTime, Iterations);

	// If we landed while wall running, make sure we stop wall running
	if (IsCustomMovementMode(ECustomMovementMode::CMOVE_Vaulting))
	{
		EndVaultingRun();
	}
}

void FSavedMove_My::Clear()
{
	Super::Clear();

	// Clear all values
	SavedWantsToSprint = 0;
	SavedVaultingKeysDown = 0;
	SavedMantleKeysDown = 0;
	SavedWantsToSlide = 0;
}

uint8 FSavedMove_My::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	/* There are 4 custom move flags for us to use. Below is what each is currently being used for:
	FLAG_Custom_0		= 0x10, // Sprinting
	FLAG_Custom_1		= 0x20, // Vaulting
	FLAG_Custom_2		= 0x40, // Sliding
	FLAG_Custom_3		= 0x80, // Mantle
	*/

	// Write to the compressed flags 
	if (SavedWantsToSprint)
		Result |= FLAG_Custom_0;
	if (SavedVaultingKeysDown)
		Result |= FLAG_Custom_1;
	if (SavedWantsToSlide)
		Result |= FLAG_Custom_2;
	if (SavedMantleKeysDown)
		Result |= FLAG_Custom_3;

	return Result;
}

bool FSavedMove_My::CanCombineWith(const FSavedMovePtr& NewMovePtr, ACharacter* Character, float MaxDelta) const
{
	const FSavedMove_My* NewMove = static_cast<const FSavedMove_My*>(NewMovePtr.Get());

	// As an optimization, check if the engine can combine saved moves.
	if (SavedWantsToSprint != NewMove->SavedWantsToSprint ||
		SavedVaultingKeysDown != NewMove->SavedVaultingKeysDown)
	{
		return false;
	}

	return Super::CanCombineWith(NewMovePtr, Character, MaxDelta);
}

void FSavedMove_My::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UNetworkCharMC* charMov = static_cast<UNetworkCharMC*>(Character->GetCharacterMovement());
	if (charMov)
	{
		// Copy values into the saved move
		SavedWantsToSprint = charMov->WantsToSprint;
		SavedVaultingKeysDown = charMov->VaultingKeysDown;
		SavedWantsToSlide = charMov->WantsToSlide;
		SavedMantleKeysDown = charMov->MantleKeysDown;
	}
}

void FSavedMove_My::PrepMoveFor(class ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UNetworkCharMC* charMov = Cast<UNetworkCharMC>(Character->GetCharacterMovement());
	if (charMov)
	{
		// Copt values out of the saved move
		charMov->WantsToSprint = SavedWantsToSprint;
		charMov->VaultingKeysDown = SavedVaultingKeysDown;
		charMov->WantsToSlide = SavedWantsToSlide;
		charMov->MantleKeysDown = SavedMantleKeysDown;
	}
}

FNetworkPredictionData_Client_My::FNetworkPredictionData_Client_My(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{

}

FSavedMovePtr FNetworkPredictionData_Client_My::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_My());
}