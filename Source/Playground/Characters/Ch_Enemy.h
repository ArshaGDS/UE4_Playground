// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ch_CharacterBase.h"

#include "I_EnemyInteractions.h"
#include "Ch_Enemy.generated.h"

UCLASS()
class PLAYGROUND_API ACh_Enemy : public ACh_CharacterBase, public II_EnemyInteractions
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACh_Enemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Info")
	float CharacterHealth{100.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Info")
	FString CharacterWeapon{"Rifle"};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Info")
	uint8 CharacterLevel{1};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Interface
	virtual void SetScanStatus_Implementation(const bool Status) override;
	
	// Interface
	virtual bool GetScanStatus_Implementation() const override;
	
	// Interface
	virtual uint8 GetLevel_Implementation() const override;
	
	// Interface
	virtual FEnemyInformation GetInformation_Implementation() const override;

private:
	
	bool IsScanned{false};

};
