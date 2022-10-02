// Fill out your copyright notice in the Description page of Project Settings.


#include "A_Zipline.h"

#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"

// Sets default values
AA_Zipline::AA_Zipline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Spline
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Zipline"));
	SetRootComponent(Spline);
	
	StartZipline = CreateDefaultSubobject<UBoxComponent>(TEXT("Start zipline"));
	EndZipline = CreateDefaultSubobject<UBoxComponent>(TEXT("End zipline"));
}

void AA_Zipline::BeginPlay()
{
	Super::BeginPlay();
	StartZipline->OnComponentBeginOverlap.AddDynamic(this, &AA_Zipline::StartZiplineOverlapBegin);
	EndZipline->OnComponentBeginOverlap.AddDynamic(this, &AA_Zipline::EndZiplineOverlapBegin);
	SetActorTickEnabled(false);
}

void AA_Zipline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	StartZipline->SetWorldLocation(Spline->GetWorldLocationAtSplinePoint(0));
	EndZipline->SetWorldLocation(Spline->GetWorldLocationAtSplinePoint(LastSplinePointIndex()));
}

// Called every frame
void AA_Zipline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Move(DeltaTime);
}

void AA_Zipline::Move(float DeltaTime)
{
	if (bIsStartToEnd)
	{
		MoveFromStartToEnd(DeltaTime);
	}
	else
	{
		MoveFromEndToStart(DeltaTime);
	}
}

void AA_Zipline::MoveFromStartToEnd(float DeltaTime)
{
	// Copy of right code
	if (Distance > Spline->GetSplineLength())
	{
		SetActorTickEnabled(false);
		bIsActive = false;
		return;
	}
	Distance = (DeltaTime * Speed) + Distance;
	const FTransform NewTransform = Spline->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World, false);
	OverlappedActorPtr->SetActorTransform(NewTransform);
	UE_LOG(LogTemp, Display, TEXT("Distance: %f\tLenght: %f\tSpeed: %f\tDelta Time: %f"), Distance, Spline->GetSplineLength(), Speed, DeltaTime);
}

void AA_Zipline::MoveFromEndToStart(float DeltaTime)
{
	Distance = (Distance - (Speed * DeltaTime));
	UE_LOG(LogTemp, Display, TEXT("Distance: %f\tLenght: %f"), Distance, Spline->GetSplineLength());
	if (Distance <= 0)
	{
		SetActorTickEnabled(false);
		bIsActive = false;
		return;
	}
	const FTransform NewTransform = Spline->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World, false);
	OverlappedActorPtr->SetActorTransform(NewTransform);
	//UE_LOG(LogTemp, Display, TEXT("Distance: %f\tLenght: %f\tSpeed: %f\tDelta Time: %f"), Distance, Spline->GetSplineLength(), Speed, DeltaTime);
}

int AA_Zipline::LastSplinePointIndex() 
{
	return Spline->GetNumberOfSplinePoints() - 1;
}


void AA_Zipline::StartZiplineOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && IsValid(OtherActor) && !bIsActive)
	{
		UE_LOG(LogTemp, Display, TEXT("Start to end"));
		OverlappedActorPtr = OtherActor;
		bIsStartToEnd = true;
		bIsActive = true;
		Distance = 0;
		SetActorTickEnabled(true);
	}
}

void AA_Zipline::EndZiplineOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && IsValid(OtherActor) && !bIsActive)
	{
		UE_LOG(LogTemp, Display, TEXT("End to start"));
		OverlappedActorPtr = OtherActor;
		bIsStartToEnd = false;
		bIsActive = true;
		Distance = Spline->GetSplineLength();
		SetActorTickEnabled(true);
	}
}