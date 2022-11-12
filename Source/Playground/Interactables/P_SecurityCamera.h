// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "I_SecuritycameraInteractions.h"
#include "GameFramework/Pawn.h"
#include "P_SecurityCamera.generated.h"

UCLASS()
class PLAYGROUND_API AP_SecurityCamera : public APawn, public II_SecuritycameraInteractions
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AP_SecurityCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	
	// Interface
	virtual void StopInvestigation() override;
	// Interface
	virtual void RestartInvestigation() override;
	
	UPROPERTY(EditAnywhere, Category = "Camera Rotation")
	UStaticMeshComponent* CameraMesh{};
	
	UPROPERTY()
	class URotatingMovementComponent* CameraMovement{};
	
	UPROPERTY(EditAnywhere, Category = "Camera Rotation")
	float Degree{70};
	UPROPERTY(EditAnywhere, Category = "Camera Rotation")
	float YawDirection{100};
	
	float MaximumRotationToLeft{};
	float MaximumRotationToRight{};
	
	void Investigating();
	
};
