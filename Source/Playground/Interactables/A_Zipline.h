// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineMeshComponent.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zipline | Movement")
	float Speed{100};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Zipline | Movement")
	EZiplineDirection ZiplineDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Zipline | Movement")
	float ZAxisOffset{0.f};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zipline | Collisions")
	FVector BoxExtent{32, 32, 32};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zipline | Collisions")
	bool bHiddenInGame{true};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Zipline | Collisions")
	bool bSetSplineToTopOfCollisions{false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Zipline | SplineMesh")
	UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Zipline | SplineMesh")
	TEnumAsByte<ESplineMeshAxis::Type> ForwardAxis;

private:
	
	float Distance{};
	
	UPROPERTY()
	AActor* OverlappedActorPtr;

	UPROPERTY()
	bool bIsStartToEnd{false};

	// When player is using zipline, returns true
	UPROPERTY()
	bool bIsActive{false};

	UPROPERTY()
	class USplineComponent* Spline;
	
	UPROPERTY()
	UBoxComponent* StartZipline;

	UPROPERTY()
	UBoxComponent* EndZipline;
	
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
