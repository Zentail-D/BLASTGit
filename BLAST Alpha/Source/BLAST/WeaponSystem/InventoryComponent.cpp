// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSystem/InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// set our current health to max at start
	CurrentPlayerHealth = MaxPlayerHealth;
}

void UInventoryComponent::PickupItem(AItemParent* ItemToPickup)
{
	if (Cast<AGeneratorPoints>(ItemToPickup))	// if the item being picked up is a generator point
	{
		AddGeneratorPoint();
	} else if (Cast<AModParent>(ItemToPickup))	// if the item being picked up is a mod 
	{
		// set our picking up item to have the proper instigator so it doesnt collide with player firing the mod
		ItemToPickup->SetInstigator(GetOwner()->GetInstigator());
		// Try to pickup the mod now that we now it is a mod
		PickupMod(Cast<AModParent>(ItemToPickup));
	}
}

void UInventoryComponent::FireActiveMod(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	//switch statement to handle which mod slot we actually have active right now
	switch(ActiveModSlot)
	{
		case EModSlots::Ve_DEFAULT:	// if default mod is active then fire default mod
			{
				DefaultMod->FireActiveMod(CameraComponent,MuzzleLocation);
				break;
			}
		case EModSlots::Ve_SLOT1:	// if slot 1 mod is active then fire default mod
			{
				ModSlot1->FireActiveMod(CameraComponent,MuzzleLocation);
				break;
			}
		case EModSlots::Ve_SLOT2:	// if slot 2 mod is active then fire default mod
			{
				ModSlot2->FireActiveMod(CameraComponent,MuzzleLocation);
				break;
			}
		default:
			{
				break;
			}
	}
}

void UInventoryComponent::ActiveModReleased(UCameraComponent* CameraComponent, UStaticMeshComponent* MuzzleLocation)
{
	//switch statement to handle which mod slot we actually have active right now
	switch(ActiveModSlot)
	{
		case EModSlots::Ve_DEFAULT:
			{
				if(DefaultMod)
				{
					DefaultMod->ActiveModRelease(CameraComponent,MuzzleLocation);
				}
				break;
			}
		case EModSlots::Ve_SLOT1:
			{
				if(ModSlot1)
				{
					ModSlot1->ActiveModRelease(CameraComponent,MuzzleLocation);
				}
				break;
			}
		case EModSlots::Ve_SLOT2:
			{
				if(ModSlot2)
				{
					ModSlot2->ActiveModRelease(CameraComponent,MuzzleLocation);
				}
				break;
			}
		default:
			{
				break;
			}
	}

	// update active mod after fire is released
	UpdateActiveMod();
}

void UInventoryComponent::ResetMods()
{
	if (ModSlot1)	// if something is in mod slot 1 destroy it
	{
		ModSlot1->Destroy();
	}
	if (ModSlot2)	// if something is in mod slot 2 destroy it
	{
		ModSlot2->Destroy();	
	}
	// need to make sure we set the active mod to default in case it wasn't
	SetActiveModSlot(EModSlots::Ve_DEFAULT);
}

FString UInventoryComponent::GetActiveModName() const
{

	//switch statement to handle which mod slot we actually have active right now
	switch(ActiveModSlot)
	{
	case EModSlots::Ve_DEFAULT:
		{
			return DefaultMod->GetModTypeName();
		}
	case EModSlots::Ve_SLOT1:
		{
			return ModSlot1->GetModTypeName();
		}
	case EModSlots::Ve_SLOT2:
		{
			return ModSlot2->GetModTypeName();
		}
	default:
		{
			return DefaultMod->GetModTypeName();
			break;
		}
	}
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// must spawn in our default mod so that it becomes usable
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = GetOwner()->GetInstigator();
	AModParent* ModParent = GetWorld()->SpawnActor<AModParent>(DefaultModClass,GetOwner()->GetActorLocation(), FRotator(0,0,0), SpawnParams);
	if(ModParent)
	{	// we want the default mod to stay forever so set never destroy to true and destroy the mesh component
		DefaultMod=ModParent;
		DefaultMod->bNeverDestroy = true;
		DefaultMod->MeshComponent->DestroyComponent();
	}
	
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick Tick, FActorComponentTickFunction* ThisTickFunction)
{
	if(ModSlot1)
	{
		if(ModSlot1->bReadyToDestroy)
		{
			ModSlot1->Destroy();
			ModSlot1=nullptr;
			UpdateActiveMod();
		}
	}
	if(ModSlot2)
	{
		if(ModSlot2->bReadyToDestroy)
		{
			ModSlot2->Destroy();
			ModSlot2=nullptr;
			UpdateActiveMod();
		}
	}
		
	
}

void UInventoryComponent::UpdateActiveMod()
{
	//switch statement to handle which mod slot we actually have active right now
	switch(ActiveModSlot)
	{
	case EModSlots::Ve_DEFAULT:
		{
			if (ModSlot1)
			{
				SetActiveModSlot(EModSlots::Ve_SLOT1);
			} else if (ModSlot2)
			{
				SetActiveModSlot(EModSlots::Ve_SLOT2);
			}
			break;
		}
	case EModSlots::Ve_SLOT1:
		{
			if (!ModSlot1)	// active mod is empty
			{
				if (!ModSlot2)	// other mod is empty therefore both mods are empty
				{
					SetActiveModSlot(EModSlots::Ve_DEFAULT);	// se we set our active to default
				} else
				{
					SetActiveModSlot(EModSlots::Ve_SLOT2);	// since slot one is empty and slot 2 is not set active mod to slot 2
				}
			}
			break;
		}
	case EModSlots::Ve_SLOT2:
		{
			if (!ModSlot2)	// active mod is empty
			{
				if (!ModSlot1)	// other mod is empty there for both mods are empty
				{
					SetActiveModSlot(EModSlots::Ve_DEFAULT);	// se we set our active to default
				} else
				{
					SetActiveModSlot(EModSlots::Ve_SLOT1);	// since active mod is empty and slot 1 is not set active mod to slot 1
				}
			}
			break;
		}
	default:
		{
			break;
		}
	}
}

void UInventoryComponent::PickupMod(AModParent* NewMod)
{
	// NOTE: all mods are set to never destroy so that the dont get destroyed on their timer and then get destroyed
	//		 while the player is using them
	
	//switch statement to handle which mod slot we actually have active right now
	switch(ActiveModSlot)
	{
		case EModSlots::Ve_DEFAULT:
			{
				if (!ModSlot1)	// if mod slot 1 is empty then add new mod to that slot
				{
					ModSlot1 = NewMod;
					ModSlot1->bNeverDestroy = true;
					NewMod->MeshComponent->DestroyComponent();	// make sure to get rid of the MeshComponent
				} else if (!ModSlot2)	// if mod slot 2 is empty then add new mod to that slot
				{
					ModSlot2 = NewMod;
					ModSlot2->bNeverDestroy = true;
					NewMod->MeshComponent->DestroyComponent();	// make sure to get rid of the MeshComponent
				} else
				{
					NewMod->Destroy();	// if somehow we end up here we destroy the mod
				}
				break;
			}
		case EModSlots::Ve_SLOT1:
			{
				if (!ModSlot2)	// if mod slot 2 is empty then add new mod to that slot
				{
					ModSlot2 = NewMod;
					ModSlot2->bNeverDestroy = true;
					NewMod->MeshComponent->DestroyComponent();	// make sure to get rid of the MeshComponent
				} else
				{
					NewMod->Destroy();	// we have not empty mod slots
				}
				break;
			}
		case EModSlots::Ve_SLOT2:
			{
				if (!ModSlot1)	// if mod slot 1 is empty then add new mod to that slot
				{
					ModSlot1 = NewMod;
					ModSlot1->bNeverDestroy = true;
					NewMod->MeshComponent->DestroyComponent();	// make sure to get rid of the MeshComponent
				} else
				{
					NewMod->Destroy();	// we have no empty mod slots
				}
				break;
			}
		default:
			{
				break;
			}
	}
	// after we do our pick up logic call our update active mod in case we came into this with default mod active
	// now we want to update our active mod or had to change mods at all
	UpdateActiveMod();
}