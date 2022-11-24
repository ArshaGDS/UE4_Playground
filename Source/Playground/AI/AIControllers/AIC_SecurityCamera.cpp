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
	// Configure AI sight
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (ensureMsgf(SightConfig, TEXT("[AIC_SecCamera]Can't access to SightConfig")))
	{
		SightConfig->SightRadius = DEFAULT_SIGHT_RADIUS;
		SightConfig->LoseSightRadius = DEFAULT_LOSE_SIGHT_RADIUS;
		SightConfig->PeripheralVisionAngleDegrees = DEFAULT_PERIPHERAL_VISION_ANGLE_DEGREES;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	}

	CameraPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
	// Set configuration
	if (ensureMsgf(CameraPerception, TEXT("[AIC_SecCamera]Can't access to CameraPerception")))
	{
		CameraPerception->ConfigureSense(*SightConfig);
		CameraPerception->SetDominantSense(SightConfig->GetSenseImplementation());
	}
}

ETeamAttitude::Type AAIC_SecurityCamera::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(&Other);
	return OtherTeamAgent ? FGenericTeamId::GetAttitude(GetGenericTeamId(), OtherTeamAgent->GetGenericTeamId())
		: ETeamAttitude::Hostile;
}

void AAIC_SecurityCamera::OnPerception(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Stimulus.WasSuccessfullySensed() && GetPawn())
	{
		// If lose sight
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Losed sight"));
		II_SecuritycameraInteractions::Execute_ResumeRotation(GetPawn());
		ClearFocus(EAIFocusPriority::Gameplay);
		return;
	}
	
	if (IsValid(Actor) && GetPawn() && GetPawn()->GetClass()->ImplementsInterface(UI_SecuritycameraInteractions::StaticClass()))
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

FGenericTeamId AAIC_SecurityCamera::GetGenericTeamId() const
{
	const FGenericTeamId GenericTeamId(TeamId);
	return GenericTeamId;
}
