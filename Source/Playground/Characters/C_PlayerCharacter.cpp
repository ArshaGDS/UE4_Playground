// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerCharacter.h"

#include "Ch_Enemy.h"
#include "I_PlayerInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#define MINIMUM_RATE 0.1f
#define MAXIMUM_RATE 1.0f

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
	if (ScanTimerDelegate.IsBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Scanner]Delegate binded"));
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

void AC_PlayerCharacter::TabAbility_Implementation()
{
	II_PlayerInputInterface::TabAbility_Implementation();

	if (!IsValid(GetWorld()))
	{
		UE_LOG(LogTemp, Error, TEXT("[Scanner]Can't access to UWorld global pointer."));
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
			UE_LOG(LogTemp, Warning, TEXT("[Scanner]UnPauseTimer"));
		}

		UE_LOG(LogTemp, Warning, TEXT("[Scanner]Scan mode is On"));
		
		// Set outline on all enemies

		// Set scan filter on camera
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Scanner]Scan mode is Off"));
		GetWorld()->GetTimerManager().PauseTimer(ScanTimerHandle);
		// Delete outline on all enemies

		// Delete scan filter on camera
	}
	
	// Get all enemies in the world
	/*TArray<AActor*> EnemiesArray;
	UGameplayStatics::GetAllActorsOfClass(this, ACh_Enemy::StaticClass(), EnemiesArray);*/
	// Set outline on enemy mesh
}

void AC_PlayerCharacter::InitScannerTimer()
{
	ScannerTimerRate = FMath::Clamp(ScannerTimerRate, MINIMUM_RATE, MAXIMUM_RATE);
	GetWorld()->GetTimerManager().SetTimer(ScanTimerHandle, ScanTimerDelegate, ScannerTimerRate, true);
	UE_LOG(LogTemp, Warning, TEXT("[Scanner]Scanner is initialized"));
	bIsTimerInitialized = true;
}

void AC_PlayerCharacter::Scan()
{
	UE_LOG(LogTemp, Warning, TEXT("[Scanner]Scanning..."));
	// Line trace for scan enemy 
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	FVector EndTrace = ViewPointLocation + ViewPointRotation.Vector() * 1000;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		ViewPointLocation,
		EndTrace,
		ECC_Pawn,
		Params
	);

	if (IsValid(Hit.GetActor()))
	{
		UE_LOG(LogTemp, Warning, TEXT("[Scanner]....Hitted...."));
	}
}
