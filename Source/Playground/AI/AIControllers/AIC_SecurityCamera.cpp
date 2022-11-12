// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_SecurityCamera.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Playground/Interactables/I_SecuritycameraInteractions.h"

#define DEFAULT_SIGHT_RADIUS 600.f
#define DEFAULT_LOSE_SIGHT_RADIUS 700.f
#define DEFAULT_PERIPHERAL_VISION_ANGLE_DEGREES 45.f // Half angle degrees

void AAIC_SecurityCamera::BeginPlay()
{
	Super::BeginPlay();
}

AAIC_SecurityCamera::AAIC_SecurityCamera()
{
	CameraPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	// Configure AI sight
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->SightRadius = DEFAULT_SIGHT_RADIUS;
	SightConfig->LoseSightRadius = DEFAULT_LOSE_SIGHT_RADIUS;
	SightConfig->PeripheralVisionAngleDegrees = DEFAULT_PERIPHERAL_VISION_ANGLE_DEGREES;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	// Set configuration
	CameraPerception->ConfigureSense(*SightConfig);
	CameraPerception->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AAIC_SecurityCamera::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	//UE_LOG(LogTemp, Warning, TEXT("[SecurityCamera]OnTargetPerceptionUpdated"));
	if (!Stimulus.WasSuccessfullySensed())
	{
		// If lose sight
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Losed sight"));
		II_SecuritycameraInteractions::Execute_ResumeRotation(GetPawn());
		ClearFocus(EAIFocusPriority::Gameplay);
		return;
	}
	
	if (IsValid(Actor) && GetPawn()->GetClass()->ImplementsInterface(UI_SecuritycameraInteractions::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Black, TEXT("Player detected"));
		II_SecuritycameraInteractions::Execute_StopRotation(GetPawn());
		SetFocus(Actor, EAIFocusPriority::Gameplay);
	}
}

void AAIC_SecurityCamera::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	CameraPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AAIC_SecurityCamera::OnPerception);
}
