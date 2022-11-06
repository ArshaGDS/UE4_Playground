// Fill out your copyright notice in the Description page of Project Settings.


#include "ScannerComponent.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Playground/Characters/Ch_PlayerCharacter.h"
#include "Playground/Characters/I_EnemyInteractions.h"
#include "Playground/Characters/I_UpdateScanUI.h"
#include "Playground/Characters/Ch_Enemy.h"

#define BLENDABLE_WEIGHT 1
#define MINIMUM_TIMER_RATE 0.1f
#define MAXIMUM_TIMER_RATE 1.0f
#define PROGRESS_PERCENT(x) (x / 10.f)

// Sets default values for this component's properties
UScannerComponent::UScannerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UScannerComponent::OnOwnerBeginPlay(ACh_PlayerCharacter* OwnerPointer)
{
	if (!IsValid(OwnerPointer))
	{
		UE_LOG(LogTemp, Error, TEXT("[Scanner]Can't access to parent pointer, initialization failed"));
		return;
	}
	// Set parent (Owner) pointer
	OwnerPtr = OwnerPointer;
	// Delegate scanner timer
	ScanTimerDelegate.BindUObject(this, &UScannerComponent::Scan);
	if (ScanUIClass && IsValid(OwnerPtr->GetWorld()->GetFirstPlayerController()))
	{
		// Set scanner UI class
		CharacterUI = CreateWidget(OwnerPtr->GetWorld()->GetFirstPlayerController(), ScanUIClass);
	}
	if (HighlightPP)
	{
		// Set highlight post process
		OwnerPtr->Camera->PostProcessSettings.AddBlendable(HighlightPP, BLENDABLE_WEIGHT);
	}
}

void UScannerComponent::Execute()
{
	if (!IsValid(OwnerPtr->GetWorld()))
	{
		UE_LOG(LogTemp, Error, TEXT("[Scanner]Can't access to UWorld global pointer."));
		return; // Scanning failed
	}
	
	if (!CharacterUI || !CharacterUI->GetClass()->ImplementsInterface(UI_UpdateScanUI::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("[Scanner]Can't access to scanner UI."));
		return; // Scanning failed
	}

	if (!OutlinePP)
	{
		UE_LOG(LogTemp, Error, TEXT("[Scanner]Can't access to post processors."));
		return; // Scanning failed
	}

	bIsInScanMode = !bIsInScanMode;
	if (bIsInScanMode)
	{
		// Initialize timer
		if (!bIsTimerInitialized) { InitScannerTimer(); }
		// If timer is paused, unpause it
		if (OwnerPtr->GetWorld()->GetTimerManager().IsTimerPaused(ScanTimerHandle))
		{
			OwnerPtr->GetWorld()->GetTimerManager().UnPauseTimer(ScanTimerHandle);
		}

		// Show scanner UI
		CharacterUI->AddToViewport();
		// Turn on scan camera effect
		OwnerPtr->Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
		OwnerPtr->Camera->PostProcessSettings.bOverride_SceneColorTint = true;
		// Set camera post process
		OwnerPtr->Camera->PostProcessSettings.AddBlendable(OutlinePP, BLENDABLE_WEIGHT);
	}
	else
	{
		// Pause scanner
		OwnerPtr->GetWorld()->GetTimerManager().PauseTimer(ScanTimerHandle);
		// Hide scanner UI
		CharacterUI->RemoveFromViewport();
		// Turn off scan camera effect
		OwnerPtr->Camera->PostProcessSettings.bOverride_VignetteIntensity = false;
		OwnerPtr->Camera->PostProcessSettings.bOverride_SceneColorTint = false;
		// Remove camera post process
		OwnerPtr->Camera->PostProcessSettings.RemoveBlendable(OutlinePP);
	}
}

void UScannerComponent::InitScannerTimer()
{
	ScannerTimerRate = FMath::Clamp(ScannerTimerRate, MINIMUM_TIMER_RATE, MAXIMUM_TIMER_RATE);
	OwnerPtr->GetWorld()->GetTimerManager().SetTimer(ScanTimerHandle, ScanTimerDelegate, ScannerTimerRate, true);
	bIsTimerInitialized = true;
}

void UScannerComponent::Scan()
{
	const FHitResult Hit = DrawLineTrace();
	
	if (IsValid(Hit.GetActor()) &&
		Hit.GetActor()->GetClass()->ImplementsInterface(UI_EnemyInteractions::StaticClass()))
	{
		// Check if enemy already scanned, get enemy information
		const bool bIsAlreadyScanned = II_EnemyInteractions::Execute_GetScanStatus(Hit.GetActor());
		if (bIsAlreadyScanned)
		{
			GetEnemyInformation(Hit.GetActor());
			return;
		}
		
		ScanCounter++;
		II_UpdateScanUI::Execute_ScanProgress(CharacterUI, PROGRESS_PERCENT(ScanCounter));
		
		if (ScanCounter >= ScanTime)
		{
			// Scan completed, Call "change scan status interface" on enemy class
			II_EnemyInteractions::Execute_SetScanStatus(Hit.GetActor(), true);
			GetEnemyInformation(Hit.GetActor());
		}
	}
	else if (ScanCounter > 0)
	{
		ScanCounter = 0;
	}
	else
	{
		II_UpdateScanUI::Execute_HideScannerWidgets(CharacterUI);
	}
}

FHitResult UScannerComponent::DrawLineTrace()
{
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	OwnerPtr->GetController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	FVector EndTrace = ViewPointLocation + ViewPointRotation.Vector() * 1500;
	FHitResult Hit{};
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerPtr);
	Params.AddIgnoredActor(GetOwner());

	OwnerPtr->GetWorld()->LineTraceSingleByChannel(
		Hit,
		ViewPointLocation,
		EndTrace,
		ECC_Pawn,
		Params
	);
	return Hit;
}

void UScannerComponent::GetEnemyInformation(const AActor* Actor)
{
	const FEnemyInformation Information = II_EnemyInteractions::Execute_GetInformation(Actor);
	II_UpdateScanUI::Execute_ScannedInformation(CharacterUI, Information.Health, Information.WeaponName);
}

