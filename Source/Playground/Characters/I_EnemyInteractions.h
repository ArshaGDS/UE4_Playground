// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_EnemyInteractions.generated.h"


USTRUCT(BlueprintType)
struct FEnemyInformation
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY()
	float Health{-1.f}; // Just for initialization
	UPROPERTY()
	FString WeaponName{"None"};
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_EnemyInteractions : public UInterface
{
	GENERATED_BODY()
};

class PLAYGROUND_API II_EnemyInteractions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetScanStatus(const bool Status);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool GetScanStatus() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	uint8 GetLevel() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FEnemyInformation GetInformation() const;
};
