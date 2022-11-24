// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIC_SecurityCamera.generated.h"

#define ALL_ENEMIES_TEAM_ID 2

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

	int TeamId{ALL_ENEMIES_TEAM_ID};
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
public:
	
	AAIC_SecurityCamera();
	
	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig{};
	
	UFUNCTION()
	void OnPerception(AActor* Actor, FAIStimulus Stimulus);
	
	virtual void OnPossess(APawn* InPawn) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
};
