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

	// Interface
	virtual void StopRotation_Implementation() override;
	
	// Interface
	virtual void ResumeRotation_Implementation() override;

private:

	UPROPERTY()
	UStaticMeshComponent* CameraMesh{};
	
	UPROPERTY()
	class URotatingMovementComponent* RotationComponent{};
	
	UPROPERTY(EditAnywhere, Category = "Camera Rotation")
	float Degree{70};
	
	UPROPERTY(EditAnywhere, Category = "Camera Rotation")
	float Rate{100};

	float CurrentYaw{};
	FRotator InitialRotation{};
	FTimerHandle TimerHandle{};
	
	void Investigating();
	void RotateToInitialRotation();
	/**
	 * This function checks the current grade is within a certain range.
	 * @param Value Current yaw 
	 * @param Central Initial yaw
	 * @param Range Rotation degree
	 */
	bool IsAngleInRange(const float Value, const float Central, const float Range);
};
