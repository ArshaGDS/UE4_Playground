// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScannerComponent.generated.h"

class UUserWidget;
class UCameraComponent;
class ACh_PlayerCharacter;
UCLASS( ClassGroup=(Ability), meta=(BlueprintSpawnableComponent) )
class PLAYGROUND_API UScannerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScannerComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Configuration")
	float ScannerTimerRate{0.5f};
	
	UPROPERTY(EditAnywhere, Category="Configuration")
	TSubclassOf<UUserWidget> ScanUIClass{};
	
	void OnOwnerBeginPlay(ACh_PlayerCharacter* OwnerPointer);
	void Execute();
	
private:
	
	UPROPERTY()
	UUserWidget* CharacterUI{};

	UPROPERTY()
	ACh_PlayerCharacter* OwnerPtr;
	
	bool bIsInScanMode{false};
	bool bIsTimerInitialized{false};
	uint8 ScanCounter{0};
	uint8 ScanTime{10}; // Maximum scan time
	FTimerHandle ScanTimerHandle{};
	FTimerDelegate ScanTimerDelegate{};

	FHitResult DrawLineTrace();
	void InitScannerTimer();
	void ScannerCounter();
	void Scan();
	void GetEnemyInformation(const AActor* Actor);
};
