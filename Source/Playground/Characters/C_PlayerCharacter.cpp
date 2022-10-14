// Fill out your copyright notice in the Description page of Project Settings.


#include "C_PlayerCharacter.h"

#include "I_PlayerInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AC_PlayerCharacter::AC_PlayerCharacter()
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
}

void AC_PlayerCharacter::TabAbility_Implementation()
{
	II_PlayerInputInterface::TabAbility_Implementation();

	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	GetController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	FVector EndTrace = ViewPointLocation + ViewPointRotation.Vector() * 1000;
	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		ViewPointLocation,
		EndTrace,
		ECC_Pawn,
		Params
	);

	if (IsValid(Hit.GetActor()))
	{
		UE_LOG(LogTemp, Warning, TEXT("[TabAbility]....Hitted...."));
	}
}

// Called when the game starts or when spawned
void AC_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AC_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AC_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (IsValid(GetWorld()->GetFirstPlayerController()) &&
			GetWorld()->GetFirstPlayerController()->GetClass()->ImplementsInterface(UI_PlayerInputComponent::StaticClass()))
	{
		II_PlayerInputComponent::Execute_GetPlayerInputComponent(GetWorld()->GetFirstPlayerController(), this,
			PlayerInputComponent, RotationSpeed);
	}
}