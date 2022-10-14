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

protected:

	virtual void BeginPlay() override;
	
public:

	// Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetPlayerInputComponent(ACharacter* PlayerCharacter, UInputComponent* PlayerInputComponent, const float& RotationSpeed);
	virtual void GetPlayerInputComponent_Implementation(ACharacter* PlayerCharacter, UInputComponent* PlayerInputComponent, const float& RotationSpeed) override;

	UPROPERTY(EditDefaultsOnly, Category="Fastmove config")
	TEnumAsByte<ECollisionChannel> CollisionChannel;
	UPROPERTY(EditDefaultsOnly, Category="Fastmove config")
	float LineTraceLength{100.f};
	
private:

	UPROPERTY()
	ACharacter* PlayerCharacterPtr;

	// To display debug results on screen
	UPROPERTY(EditDefaultsOnly, Category="Fastmove config")
	bool bIsDebugging{false};
	
	float RotationSpeedRef;
	uint8 KeyPressedCount{};
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;

	// Initialize inputs
	void SetupInputAxis(UInputComponent* PlayerInputComponent);
	void SetupInputActions(UInputComponent* PlayerInputComponent);
	
	// Set movement inputs
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);	
	void Turn(float Rate);	
	void LookUp(float Rate);
	
	// Calculates double taps on the w key in a specified time
	void ReleasedW();
	void ResetKeyPressedCount();
	// Fast movement feature
	void FastMovement();
	void TabKeyAbility();
};
