// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "A_Zipline.generated.h"

UENUM()
enum class EZiplineDirection { OneWay, TwoWay };

class UBoxComponent;
UCLASS()
class PLAYGROUND_API AA_Zipline : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_Zipline();

protected:

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zipline")
	float Speed{100};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Zipline")
	class USplineComponent* Spline;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Zipline")
	UBoxComponent* StartZipline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Zipline")
	UBoxComponent* EndZipline;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Zipline")
	EZiplineDirection ZiplineDirection;
	
private:
	
	float Distance{};
	
	UPROPERTY()
	AActor* OverlappedActorPtr;

	UPROPERTY()
	bool bIsStartToEnd{false};

	// When player is using zipline, returns true
	UPROPERTY()
	bool bIsActive{false};
	
	UFUNCTION()
	void Move(float DeltaTime);

	UFUNCTION()
	void MoveFromStartToEnd(const float DeltaTime);

	UFUNCTION()
	void MoveFromEndToStart(const float DeltaTime);

	UFUNCTION()
	int LastSplinePointIndex() const;
	
	UFUNCTION()
	void StartZiplineOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void EndZiplineOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
