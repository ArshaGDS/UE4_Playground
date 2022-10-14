// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Default.h"

#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"
#include "I_PlayerInputInterface.h"

void APC_Default::BeginPlay()
{
	Super::BeginPlay();
	// Bind ResetKeyPressedCount function to reset KeyPressedCount to zero after specific time
	TimerDelegate.BindUObject(this, &APC_Default::ResetKeyPressedCount);
}

// Interface
void APC_Default::GetPlayerInputComponent_Implementation(ACharacter* PlayerCharacter,
	UInputComponent* PlayerInputComponent, const float& RotationSpeed)
{
	II_PlayerInputComponent::GetPlayerInputComponent_Implementation(PlayerCharacter, PlayerInputComponent,
	                                                                RotationSpeed);
	RotationSpeedRef = RotationSpeed;
	PlayerCharacterPtr = PlayerCharacter;
	
	SetupInputAxis(PlayerInputComponent);
	SetupInputActions(PlayerInputComponent);
}

void APC_Default::SetupInputAxis(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis("MoveForward", this, &APC_Default::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APC_Default::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APC_Default::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &APC_Default::LookUp);
}

void APC_Default::SetupInputActions(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("FastMovement", IE_Released, this, &APC_Default::ReleasedW);
	PlayerInputComponent->BindAction("TabAbility", IE_Pressed, this, &APC_Default::TabKeyAbility);
}

void APC_Default::MoveForward(float AxisValue)
{
	if (AxisValue != 0)
	{
		PlayerCharacterPtr->AddMovementInput(PlayerCharacterPtr->GetActorForwardVector() * AxisValue);
	}
}

void APC_Default::ReleasedW()
{
	// Calculates double taps on the w key in a specified time
	if (KeyPressedCount < 2)
	{
		KeyPressedCount++;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.2f, false);
	}
	if (KeyPressedCount == 2)
	{
		ResetKeyPressedCount();
		FastMovement();
	}
}

void APC_Default::ResetKeyPressedCount()
{
	KeyPressedCount = 0;
}

void APC_Default::FastMovement()
{
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	PlayerCharacterPtr->Controller->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	const FVector EndLocation = ViewPointLocation + ViewPointRotation.Vector() * LineTraceLength;
	if(bIsDebugging)
	{
		DrawDebugLine(
		   GetWorld(),
		   ViewPointLocation,
		   EndLocation,
		   FColor::Red,
		   false,
		   2.f,
		   2.f,
		   4
	   );
	}
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerCharacterPtr);
	FHitResult Hit;
	const bool bSuccess = GetWorld()->LineTraceSingleByChannel(
		Hit, ViewPointLocation, EndLocation, CollisionChannel, QueryParams);
	if (bSuccess && Hit.Actor.IsValid())
	{
		PlayerCharacterPtr->SetActorLocation(Hit.Location);
		
		if(bIsDebugging)
		{
			DrawDebugSphere(
			   GetWorld(),
			   Hit.Location,
			   15.f,
			   10.f,
			   FColor::Red,
			   false,
			   2.f,
			   2.f
			   );
		}
	}
}

void APC_Default::TabKeyAbility()
{
	if (IsValid(PlayerCharacterPtr) && PlayerCharacterPtr->GetClass()->ImplementsInterface(UI_PlayerInputInterface::StaticClass()))
	{
		II_PlayerInputInterface::Execute_TabAbility(PlayerCharacterPtr);
	}
}

void APC_Default::MoveRight(float AxisValue)
{
	if (AxisValue != 0)
	{
		PlayerCharacterPtr->AddMovementInput(PlayerCharacterPtr->GetActorRightVector() * AxisValue);
	}
}

void APC_Default::Turn(float Rate)
{
	if (Rate != 0)
	{
		PlayerCharacterPtr->AddControllerYawInput(Rate * RotationSpeedRef * GetWorld()->GetDeltaSeconds());
	}
}

void APC_Default::LookUp(float Rate)
{
	if (Rate != 0)
	{
		PlayerCharacterPtr->AddControllerPitchInput(Rate * RotationSpeedRef * GetWorld()->GetDeltaSeconds());	
	}
}
