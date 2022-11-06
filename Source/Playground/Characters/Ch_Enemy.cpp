// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_Enemy.h"
#include "Components/PostProcessComponent.h"

#define HIGHLIGHT_DEPTH_VALUE 4

// Sets default values
ACh_Enemy::ACh_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACh_Enemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACh_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACh_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
// Interface
void ACh_Enemy::SetScanStatus_Implementation(const bool Status)
{
	IsScanned = Status;
	if (IsScanned)
	{
		GetMesh()->SetCustomDepthStencilValue(HIGHLIGHT_DEPTH_VALUE);
	}
}
// Interface
bool ACh_Enemy::GetScanStatus_Implementation() const
{
	return IsScanned;
}
// Interface
uint8 ACh_Enemy::GetLevel_Implementation() const
{
	return CharacterLevel;
}
// Interface
FEnemyInformation ACh_Enemy::GetInformation_Implementation() const
{
	return FEnemyInformation{CharacterHealth, CharacterWeapon};
}
