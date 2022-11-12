// Fill out your copyright notice in the Description page of Project Settings.


#include "P_SecurityCamera.h"

#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
AP_SecurityCamera::AP_SecurityCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraMesh  = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Camera Mesh"));
	CameraMesh->SetupAttachment(RootComponent);
	
	CameraMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Camera movement component"));
}

// Called when the game starts or when spawned
void AP_SecurityCamera::BeginPlay()
{
	Super::BeginPlay();
	
	MaximumRotationToLeft  = GetActorRotation().Yaw + Degree;
	MaximumRotationToRight = GetActorRotation().Yaw - Degree;
	
	CameraMovement->RotationRate.Yaw = YawDirection;
	UE_LOG(LogTemp, Display, TEXT("[SecurityCamera]Initial Yaw: %f"), GetActorRotation().Yaw);
}

// Called every frame
void AP_SecurityCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Change the function's name
	Investigating();
}

// Called to bind functionality to input
void AP_SecurityCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
// Interface
void AP_SecurityCamera::StopInvestigation()
{
	II_SecuritycameraInteractions::StopInvestigation();
	CameraMovement->Deactivate();
	SetActorTickEnabled(false);
}
// Interface
void AP_SecurityCamera::RestartInvestigation()
{
	II_SecuritycameraInteractions::RestartInvestigation();
	CameraMovement->Activate();
	SetActorTickEnabled(true);
}

void AP_SecurityCamera::Investigating()
{
	const float CurrentYaw = GetActorRotation().Yaw;
	if (CurrentYaw >= MaximumRotationToLeft || CurrentYaw <= MaximumRotationToRight)
	{
		// Change movement direction
		CameraMovement->RotationRate.Yaw *= -1;
	}
}

