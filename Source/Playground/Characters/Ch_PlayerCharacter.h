// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "Playground/PlayerControllers/I_PlayerInputInterface.h"
#include "Ch_PlayerCharacter.generated.h"

class UUserWidget;

UCLASS()
class PLAYGROUND_API ACh_PlayerCharacter : public ACharacter, public  II_PlayerInputInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACh_PlayerCharacter();

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
	
	UPROPERTY(EditAnywhere)
	class UScannerComponent* ScannerComponent;
	
	// Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ScanAbility();
	virtual void ScanAbility_Implementation() override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	
	FHitResult DrawLineTrace();
};