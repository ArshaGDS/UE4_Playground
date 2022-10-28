// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerCharacter.h"

#include "Ch_Enemy.h"
#include "I_PlayerInputComponent.h"
#include "I_UpdateScanUI.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

#define MINIMUM_TIMER_RATE 0.1f
#define MAXIMUM_TIMER_RATE 1.0f
#define PROGRESS_PERCENT(x) (x / 10.f)

// Sets default values
AC_PlayerCharacter::AC_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(CameraPosition);

	GetMesh()->SetupAttachment(Camera);
	GetMesh()->SetOnlyOwnerSee(true);
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;
}

// Called when the game starts or when spawned
void AC_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	ScanTimerDelegate.BindUObject(this, &AC_PlayerCharacter::Scan);
	if (!ScanUIClass)
	{
		CharacterUI = CreateWidget(GetWorld()->GetFirstPlayerController(), ScanUIClass);
	}
}

// Called every frame
void AC_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AC_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (IsValid(GetWorld()->GetFirstPlayerController()) &&
			GetWorld()->GetFirstPlayerController()->GetClass()->ImplementsInterface(UI_PlayerInputComponent::StaticClass()))
	{
		II_PlayerInputComponent::Execute_GetPlayerInputComponent(GetWorld()->GetFirstPlayerController(), this,
			PlayerInputComponent, RotationSpeed);
	}
}

bool AC_PlayerCharacter::ScanAbility_Implementation()
{
	if (!IsValid(GetWorld()))
	{
		UE_LOG(LogTemp, Error, TEXT("[Scanner]Can't access to UWorld global pointer."));
		return false; // Scanning failed
	}
	
	if (!CharacterUI || !CharacterUI->GetClass()->ImplementsInterface(UI_UpdateScanUI::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("[Scanner]Can't access to scanner UI."));
		return;
	}

	bIsInScanMode = !bIsInScanMode;
	if (bIsInScanMode)
	{
		// Initialize timer
		if (!bIsTimerInitialized) { InitScannerTimer(); }
		// If timer is paused, unpause it
		if (GetWorld()->GetTimerManager().IsTimerPaused(ScanTimerHandle))
		{
			GetWorld()->GetTimerManager().UnPauseTimer(ScanTimerHandle);
		}
		
		CharacterUI->AddToViewport();
		// Turn on scan camera effect
		Camera->PostProcessSettings.bOverride_VignetteIntensity = true;
		Camera->PostProcessSettings.bOverride_SceneColorTint = true;
	}
	else
	{
		GetWorld()->GetTimerManager().PauseTimer(ScanTimerHandle);
		CharacterUI->RemoveFromViewport();
		// Turn off scan camera effect
		Camera->PostProcessSettings.bOverride_VignetteIntensity = false;
		Camera->PostProcessSettings.bOverride_SceneColorTint = false;
	}

	return bIsInScanMode;
}

FHitResult AC_PlayerCharacter::DrawLineTrace()
{
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	FVector EndTrace = ViewPointLocation + ViewPointRotation.Vector() * 1500;
	FHitResult Hit{};
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(
		Hit,
		ViewPointLocation,
		EndTrace,
		ECC_Pawn,
		Params
	);
	
	return Hit;
}

void AC_PlayerCharacter::InitScannerTimer()
{
	ScannerTimerRate = FMath::Clamp(ScannerTimerRate, MINIMUM_TIMER_RATE, MAXIMUM_TIMER_RATE);
	GetWorld()->GetTimerManager().SetTimer(ScanTimerHandle, ScanTimerDelegate, ScannerTimerRate, true);
	bIsTimerInitialized = true;
}

void AC_PlayerCharacter::Scan()
{
		
	const FHitResult Hit = DrawLineTrace();
	
	if (IsValid(Hit.GetActor()) &&
		Hit.GetActor()->GetClass()->ImplementsInterface(UI_EnemyInteractions::StaticClass()))
	{
		// Check if enemy already scanned, get enemy information
		// Calculate scan time with this function => CalculateScanTime(Hit.GetActor());
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
		II_UpdateScanUI::Execute_HideScanWidgets(CharacterUI);
	}
}

void AC_PlayerCharacter::CalculateScanTime(const AActor* Actor)
{
	if (Actor->GetClass()->ImplementsInterface(UI_EnemyInteractions::StaticClass()))
	{
		const uint8 EnemyLevel = II_EnemyInteractions::Execute_GetLevel(Actor);
		int8 a = EnemyLevel - 1;
	}
}

void AC_PlayerCharacter::GetEnemyInformation(const AActor* Actor)
{
	const FEnemyInformation Information = II_EnemyInteractions::Execute_GetInformation(Actor);
	II_UpdateScanUI::Execute_ScannedInformation(CharacterUI, Information.Health, Information.WeaponName);
}


