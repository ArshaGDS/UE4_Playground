// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ch_Enemy.generated.h"

UCLASS()
class PLAYGROUND_API ACh_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACh_Enemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character info")
	float CharacterHealth{100.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character info")
	FString CharacterWeapon{"Rifle"};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};