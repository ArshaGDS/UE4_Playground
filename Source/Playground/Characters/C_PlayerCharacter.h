// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Playground/PlayerControllers/I_PlayerInputInterface.h"
#include "C_PlayerCharacter.generated.h"

UCLASS()
class PLAYGROUND_API AC_PlayerCharacter : public ACharacter, public  II_PlayerInputInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AC_PlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditDefaultsOnly, Category="Character")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Movement")
	float RotationSpeed = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Camera")
	FVector CameraPosition = FVector(-39.56f, 1.75f, 64.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Scanner")
	float ScannerTimerRate{0.5f};

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TabAbility();
	virtual void TabAbility_Implementation() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	bool bIsInScanMode{false};
	float ScanCounter{0.f};
	FTimerHandle ScanTimerHandle{};
	FTimerDelegate ScanTimerDelegate;
	bool bIsTimerInitialized{false};
	
	void InitScannerTimer();
	void Scan();
};
