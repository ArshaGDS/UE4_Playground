// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_SecurityCamera.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Playground/Interactables/I_SecuritycameraInteractions.h"

#define DEFAULT_SIGHT_RADIUS 100.f
#define DEFAULT_LOSE_SIGHT_RADIUS 150.f
#define DEFAULT_PERIPHERAL_VISION_ANGLE_DEGREES 90.f

void AAIC_SecurityCamera::BeginPlay()
{
	Super::BeginPlay();
}

AAIC_SecurityCamera::AAIC_SecurityCamera()
{
	CameraPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->SightRadius = DEFAULT_SIGHT_RADIUS;
	SightConfig->LoseSightRadius = DEFAULT_LOSE_SIGHT_RADIUS;
	SightConfig->PeripheralVisionAngleDegrees = DEFAULT_PERIPHERAL_VISION_ANGLE_DEGREES;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	CameraPerception->ConfigureSense(*SightConfig);
	CameraPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AAIC_SecurityCamera::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	if (IsValid(Actor) && Actor->GetClass()->ImplementsInterface(UI_SecuritycameraInteractions::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Black, TEXT("Interface implemented"));
	}
}

void AAIC_SecurityCamera::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CameraPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AAIC_SecurityCamera::OnPerception);
}
