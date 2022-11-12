// Fill out your copyright notice in the Description page of Project Settings.


#include "P_SecurityCamera.h"
#include "GameFramework/RotatingMovementComponent.h"

#define OUTLINE_DEPTH_VALUE 1

// Sets default values
AP_SecurityCamera::AP_SecurityCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Camera Mesh"));
	CameraMesh->bRenderCustomDepth = true;
	CameraMesh->SetCustomDepthStencilValue(OUTLINE_DEPTH_VALUE);
	CameraMesh->SetupAttachment(RootComponent);
	
	RotationComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Camera Movement Component"));
}

// Called when the game starts or when spawned
void AP_SecurityCamera::BeginPlay()
{
	Super::BeginPlay();
	
	RotationComponent->RotationRate.Yaw = Rate;
	bUseControllerRotationYaw = false;
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
	RotationComponent->Deactivate();
	bUseControllerRotationYaw = true;
}

// Interface
void AP_SecurityCamera::ResumeRotation_Implementation()
{
	bUseControllerRotationYaw = false;

	if (IsAngleInRange(CurrentYaw, InitialRotation.Yaw, Degree))
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, TEXT("In range"));
		RotationComponent->Activate();
		return;
	}

	// Current Yaw is out of range
	if (GetWorld())
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, TEXT("Current Yaw is out of range"));
		TimerHandle = FTimerHandle();
		FTimerDelegate TimerDelegate{};
		TimerDelegate.BindUObject(this, &AP_SecurityCamera::RotateToInitialRotation);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, GetWorld()->DeltaTimeSeconds, true);
	}
}

void AP_SecurityCamera::Investigating()
{
	CurrentYaw = GetActorRotation().Yaw;
	
	if (!IsAngleInRange(CurrentYaw, InitialRotation.Yaw, Degree))
	{
		// Change movement direction
		RotationComponent->RotationRate.Yaw *= -1;
	}
}

bool AP_SecurityCamera::IsAngleInRange(const float Value, const float Central, const float Range)
{
	return FMath::Cos(FMath::DegreesToRadians(Value) - FMath::DegreesToRadians(Central)) >= FMath::Cos(FMath::DegreesToRadians(Range));
}

void AP_SecurityCamera::RotateToInitialRotation()
{
	GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Magenta, TEXT(__FUNCTION__));
	const FRotator Rotation = FMath::RInterpConstantTo(
		GetActorRotation(),
		InitialRotation,
		GetWorld()->DeltaTimeSeconds,
		Rate
	);
	SetActorRotation(Rotation);

	if (FMath::IsNearlyEqual(CurrentYaw, InitialRotation.Yaw, 2))
	{
		GetWorld()->GetTimerManager().PauseTimer(TimerHandle);
		TimerHandle.Invalidate();
		RotationComponent->Activate();
	}
}
