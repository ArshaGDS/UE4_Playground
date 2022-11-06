// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_PlayerCharacter.h"

#include "Ch_Enemy.h"
#include "I_PlayerInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "CharacterComponents/ScannerComponent.h"

#define MINIMUM_TIMER_RATE 0.1f
#define MAXIMUM_TIMER_RATE 1.0f
#define PROGRESS_PERCENT(x) (x / 10.f)

// Sets default values
ACh_PlayerCharacter::ACh_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = true;
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(CameraPosition);

	GetMesh()->SetupAttachment(Camera);
	GetMesh()->SetOnlyOwnerSee(true);
	GetMesh()->bCastDynamicShadow = false;
	GetMesh()->CastShadow = false;

	ScannerComponent = CreateDefaultSubobject<UScannerComponent>(TEXT("Scanner Component"));
}

// Called when the game starts or when spawned
void ACh_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	ScannerComponent->OnOwnerBeginPlay(this);
}

// Called every frame
void ACh_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACh_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (IsValid(GetController()) &&
			GetController()->GetClass()->ImplementsInterface(UI_PlayerInputComponent::StaticClass()))
	{
		II_PlayerInputComponent::Execute_GetPlayerInputComponent(GetController(), this, PlayerInputComponent, RotationSpeed);
	}
}

void ACh_PlayerCharacter::ScanAbility_Implementation()
{
	ScannerComponent->Execute();
}

FHitResult ACh_PlayerCharacter::DrawLineTrace()
{
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	FVector EndTrace = ViewPointLocation + ViewPointRotation.Vector() * 1500;
	FHitResult Hit{};
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(
		Hit,
		ViewPointLocation,
		EndTrace,
		ECC_Pawn,
		Params
	);
	
	return Hit;
}