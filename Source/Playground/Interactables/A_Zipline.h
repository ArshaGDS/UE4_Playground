// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "A_Zipline.generated.h"

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zipline")
	UBoxComponent* StartZipline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zipline")
	UBoxComponent* EndZipline;

private:
	
	float Distance{};
	
	UPROPERTY()
	AActor* OverlappedActorPtr;

	UPROPERTY()
	bool bIsStartToEnd{false};

	UPROPERTY()
	bool bIsActive{false};
	
	UFUNCTION()
	void Move(float DeltaTime);

	UFUNCTION()
	void MoveFromStartToEnd(float DeltaTime);

	UFUNCTION()
	void MoveFromEndToStart(float DeltaTime);

	UFUNCTION()
	int LastSplinePointIndex();
	
	UFUNCTION()
	void StartZiplineOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void EndZiplineOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};
