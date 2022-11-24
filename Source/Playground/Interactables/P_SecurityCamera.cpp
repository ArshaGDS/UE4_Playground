// Fill out your copyright notice in the Description page of Project Settings.


#include "P_SecurityCamera.h"
#include "GameFramework/RotatingMovementComponent.h"

#define OUTLINE_DEPTH_VALUE 1
#define ROTATION_ERROR_TOLERANCE 2

// Sets default values
AP_SecurityCamera::AP_SecurityCamera()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Camera Mesh"));
	if (CameraMesh)
	{
		CameraMesh->bRenderCustomDepth = true;
		CameraMesh->SetCustomDepthStencilValue(OUTLINE_DEPTH_VALUE);
		CameraMesh->SetupAttachment(RootComponent);
	}
	
	RotationComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Camera Movement Component"));
}

// Called when the game starts or when spawned
void AP_SecurityCamera::BeginPlay()
{
	Super::BeginPlay();
	
	bUseControllerRotationYaw = false;
	SetYawRotationRate(Rate);
	InitialRotation = GetActorRotation();
}

// Called every frame
void AP_SecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Investigating();
}

// Called to bind functionality to input
void AP_SecurityCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Interface
void AP_SecurityCamera::StopRotation_Implementation()
{
	SetRotationActivityState(false);
	bUseControllerRotationYaw = true;
}

// Interface
void AP_SecurityCamera::ResumeRotation_Implementation()
{
	bUseControllerRotationYaw = false;
	SetRotationActivityState(true);
}

void AP_SecurityCamera::Investigating()
{
	CurrentYaw = GetActorRotation().Yaw;
	
	if (IsAngleInRange(CurrentYaw, InitialRotation.Yaw, Degree))
	{
		bIsLastAngleInRange = true;
	}
	else
	{
		if (bIsLastAngleInRange)
		{
			// Last angle is in range but current angle is out of range. Turn back to the range. 
			// Change movement direction
			ReverseTheRotation();
		}
		bIsLastAngleInRange = false;
	}
}

bool AP_SecurityCamera::IsAngleInRange(const float Value, const float Central, const float Range) const
{
	if (GEngine && bShowDebugMsg)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Red,
			FString::Printf(
					TEXT("Value: %f | D2R Value: %f | Central: %f | D2R Central: %f | 1st formula: %f | 2th formula: %f"),
						Value,
						FMath::DegreesToRadians(Value),
						Central,
						FMath::DegreesToRadians(Central),
						FMath::Cos(FMath::DegreesToRadians(Value) - FMath::DegreesToRadians(Central)),
						FMath::Cos(FMath::DegreesToRadians(Range))
					)
		);
		
	}
	return FMath::Cos(FMath::DegreesToRadians(Value) - FMath::DegreesToRadians(Central)) >= FMath::Cos(FMath::DegreesToRadians(Range));
}

void AP_SecurityCamera::SetYawRotationRate(const float RotationRate) const
{
	if (ensureMsgf(RotationComponent, TEXT("%s Can't access to rotation component."), *GetActorLabel()))
	{
		RotationComponent->RotationRate.Yaw = RotationRate;
	}
}

void AP_SecurityCamera::ReverseTheRotation() const
{
	if (ensureMsgf(RotationComponent, TEXT("%s Can't access to rotation component."), *GetActorLabel()))
	{
		RotationComponent->RotationRate.Yaw *= -1;
	}
}

void AP_SecurityCamera::SetRotationActivityState(const bool State) const
{
	if (ensureMsgf(RotationComponent, TEXT("%s Can't access to rotation component."), *GetActorLabel()))
	{
		if (State)
		{
			RotationComponent->Activate();
		}
		else
		{
			RotationComponent->Deactivate();
		}
	}
}