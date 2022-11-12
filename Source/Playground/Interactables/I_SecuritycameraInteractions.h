// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "I_SecuritycameraInteractions.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UI_SecuritycameraInteractions : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PLAYGROUND_API II_SecuritycameraInteractions
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopRotation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResumeRotation();
};
