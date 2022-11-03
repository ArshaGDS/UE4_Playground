// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "I_EnemyInteractions.h"
#include "GameFramework/Character.h"
#include "Ch_Enemy.generated.h"

UCLASS()
class PLAYGROUND_API ACh_Enemy : public ACharacter, public II_EnemyInteractions
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACh_Enemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character info")
	float CharacterHealth{100.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character info")
	FString CharacterWeapon{"Rifle"};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character info")
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetScanStatus(const bool Status);
	virtual void SetScanStatus_Implementation(const bool Status) override;
	
	// Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetScanStatus() const;
	virtual bool GetScanStatus_Implementation() const override;
	
	// Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	uint8 GetLevel() const;
	virtual uint8 GetLevel_Implementation() const override;
	
	// Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FEnemyInformation GetInformation() const;
	virtual FEnemyInformation GetInformation_Implementation() const override;

private:
	
	bool IsScanned{false};

};
