// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameUI.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"

UInGameUI::UInGameUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D>TextureAsset(TEXT("Texture2D'/Game/BLAST/Gameplay/WepSys/crosshair.crosshair'"));
	CrossHairTexture = TextureAsset.Object;
}
void UInGameUI::NativeConstruct()
{
	Super::NativeConstruct();
	
		
	//positions are based on top left corner of screen
	//setup ammo counts, may add for differentiation( when graphics designer comes along.
	AmmoSlotOne = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("AmmoOne"));
	AmmoSlotOne->SetRenderTransform(FWidgetTransform(FVector2D(1763.0f,976.0f),FVector2D(151.0f,40.0f),FVector2D(0.0f,0.0f),0.0f));
	AmmoSlotTwo = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("AmmoTwo"));
	AmmoSlotTwo->SetRenderTransform(FWidgetTransform(FVector2D(1763.0f,1032.0f),FVector2D(151.0f,40.0f),FVector2D(0.0f,0.0f),0.0f));
	//setup crosshair stuff
	CrossHair = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("CrossHair"));
	
	CrossHair->SetBrushFromTexture(CrossHairTexture);
	CrossHair->SetRenderTransform(FWidgetTransform(FVector2D(941.0f,520.5f),FVector2D(40.0f,40.0f),FVector2D(0.0f,0.0f),0.0f));
	ChargeTime = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("ChargeTimeBar"));
	ChargeTime->SetRenderTransform(FWidgetTransform(FVector2D(942.0f,560.5f),FVector2D(40.0f,20.0f),FVector2D(0.0f,0.0f),0.0f));
	ChargeTime->SetFillColorAndOpacity(FLinearColor::Blue);
	//setup health stuff
	Health = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("Healthbar"));
	Health->SetRenderTransform(FWidgetTransform(FVector2D(961.0f,1027.0f),FVector2D(867.5f,40.0f),FVector2D(0.0f,0.0f),0.0f));
	Health->SetFillColorAndOpacity(FLinearColor::Red);
	CurrentHealth = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("CurrentHealth"));
	CurrentHealth->SetRenderTransform(FWidgetTransform(FVector2D(800.7f,1027.0f),FVector2D(151.0f,40.0f),FVector2D(0.0f,0.0f),0.0f));
	OutOf = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("/"));
	OutOf->SetText(FText::FromString("/"));
	OutOf->SetRenderTransform(FWidgetTransform(FVector2D(961.0f,1027.0f),FVector2D(16.8f,40.0f),FVector2D(0.0f,0.0f),0.0f));
	MaxHealth = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("MaxHealth"));
	MaxHealth->SetRenderTransform(FWidgetTransform(FVector2D(975.7f,1027.0f),FVector2D(151.0f,40.0f),FVector2D(0.0f,0.0f),0.0f));

}

void UInGameUI::UpdateAmmoUI(float NewAmmoCount, int bActiveAmmo)
{
	if(AmmoSlotOne&&AmmoSlotTwo)
	{
		if(bActiveAmmo==1)
		{
			AmmoSlotOne->SetText(FText::FromString(FString::SanitizeFloat(NewAmmoCount)));
		}
		else if(bActiveAmmo ==2)
		{
			AmmoSlotTwo->SetText(FText::FromString(FString::SanitizeFloat(NewAmmoCount)));
		}
	}
}

void UInGameUI::UpdateChargeUI(float NewChargeTime,float MaxChargeTime)
{
	if(ChargeTime)
	{
		if (ChargeTime->Visibility == ESlateVisibility::Hidden)
		{
			ChargeTime->SetVisibility(ESlateVisibility::Visible);
		}
		ChargeTime->SetPercent(NewChargeTime/MaxChargeTime);
	}
}
void UInGameUI::NotCharging()
{
	if(ChargeTime)
	{
		ChargeTime->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInGameUI::UpdateHealthUI(float newCurrentHealth, float newMaxHealth)
{
	if(CurrentHealth&&MaxHealth&&Health)
	{
		CurrentHealth->SetText(FText::FromString(FString::SanitizeFloat(newCurrentHealth)));
		MaxHealth->SetText(FText::FromString(FString::SanitizeFloat(newMaxHealth)));
		Health->SetPercent(newCurrentHealth/newMaxHealth);
	}
}
