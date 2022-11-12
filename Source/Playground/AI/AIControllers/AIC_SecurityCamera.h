// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIC_SecurityCamera.generated.h"

/**
 * 
 */
UCLASS()
class PLAYGROUND_API AAIC_SecurityCamera : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere)
	UAIPerceptionComponent* CameraPerception{};
	
	virtual void BeginPlay() override;
	
public:
	
	AAIC_SecurityCamera();
	
	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig{};
	
	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);
	
	virtual void OnPossess(APawn* InPawn) override;
};
