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
	class UCameraComponent* Camera{nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Camera")
	FVector CameraPosition{-39.56f, 1.75f, 64.f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Movement")
	float RotationSpeed{100.f};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character | Scanner")
	float ScannerTimerRate{0.5f};
	
	// Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ScanAbility();
	virtual bool ScanAbility_Implementation() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	bool bIsInScanMode{false};
	bool bIsTimerInitialized{false};
	uint8 ScanCounter{0};
	uint8 ScanTime{5}; // TODO: It's just for debugging, Change it to 0 for real gameplay
	FTimerHandle ScanTimerHandle{};
	FTimerDelegate ScanTimerDelegate{};

	FHitResult DrawLineTrace();
	void InitScannerTimer();
	void Scan();
	void CalculateScanTime(const AActor* Actor);
	void GetEnemyInformation(const AActor* Actor);
};
