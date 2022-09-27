// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Playground/Characters/I_PlayerInputComponent.h"
#include "PC_Default.generated.h"

/**
 * 
 */
UCLASS()
class PLAYGROUND_API APC_Default : public APlayerController, public II_PlayerInputComponent
{
	GENERATED_BODY()

public:

	// Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetPlayerInputComponent(ACharacter* PlayerCharacter, UInputComponent* PlayerInputComponent, const float& RotationSpeed);
	virtual void GetPlayerInputComponent_Implementation(ACharacter* PlayerCharacter, UInputComponent* PlayerInputComponent, const float& RotationSpeed) override;

private:

	UPROPERTY()
	ACharacter* PlayerCharacterPtr;
	
	UPROPERTY()
	float RotationSpeedRef;
	
	void SetupInputAxis(UInputComponent* PlayerInputComponent);
	void MoveForward(float AxisValue);	
	void MoveRight(float AxisValue);	
	void Turn(float Rate);	
	void LookUp(float Rate);	
};
