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
	// We only need the tick function when the zipline is in use
	SetActorTickEnabled(false);
	StartZipline->OnComponentBeginOverlap.AddDynamic(this, &AA_Zipline::StartZiplineOverlapBegin);
	if (ZiplineDirection == EZiplineDirection::OneWay)
	{
		EndZipline->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		return;
	}
	// If we are going to have a two-way zip line
	EndZipline->OnComponentBeginOverlap.AddDynamic(this, &AA_Zipline::EndZiplineOverlapBegin);
}	

void AA_Zipline::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (SplineMesh)
	{
		// Creates a spline mesh along the spline
		for (uint8 SplineIndex{}; SplineIndex <= LastSplinePointIndex(); SplineIndex++)
		{
			CreateNewSplineMesh(SplineIndex);
		}
	}
	// Sets the collision box size
	StartZipline->SetBoxExtent(BoxExtent);
	EndZipline->SetBoxExtent(BoxExtent);
	
	StartZipline->SetHiddenInGame(bHiddenInGame);
	EndZipline->SetHiddenInGame(bHiddenInGame);
	
	StartZipline->SetWorldLocation(
		bSetSplineToTopOfCollisions ?
		FVector {
			Spline->GetWorldLocationAtSplinePoint(0).X,
			Spline->GetWorldLocationAtSplinePoint(0).Y,
			Spline->GetWorldLocationAtSplinePoint(0).Z - StartZipline->GetScaledBoxExtent().Z
		} :
		Spline->GetWorldLocationAtSplinePoint(0)
	);
	
	EndZipline->SetWorldLocation(
		bSetSplineToTopOfCollisions ?
		FVector {
			Spline->GetWorldLocationAtSplinePoint(LastSplinePointIndex()).X,
			Spline->GetWorldLocationAtSplinePoint(LastSplinePointIndex()).Y,
			Spline->GetWorldLocationAtSplinePoint(LastSplinePointIndex()).Z - EndZipline->GetScaledBoxExtent().Z
		} :
		Spline->GetWorldLocationAtSplinePoint(LastSplinePointIndex())
	);
	
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

void AA_Zipline::MoveFromStartToEnd(const float DeltaTime)
{
	if (Distance > Spline->GetSplineLength())
	{
		SetActorTickEnabled(false);
		bIsActive = false;
		return;
	}
	Distance = (DeltaTime * Speed) + Distance;
	FVector NewActorLocation = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	NewActorLocation.Z += ZAxisOffset;
	OverlappedActorPtr->SetActorLocation(NewActorLocation);
	//UE_LOG(LogTemp, Display, TEXT("Distance: %f\tLenght: %f\tSpeed: %f\tDelta Time: %f"), Distance, Spline->GetSplineLength(), Speed, DeltaTime);
}

void AA_Zipline::MoveFromEndToStart(const float DeltaTime)
{
	Distance = Distance - (Speed * DeltaTime);
	//UE_LOG(LogTemp, Display, TEXT("Distance: %f\tLenght: %f"), Distance, Spline->GetSplineLength());
	if (Distance <= 0)
	{
		SetActorTickEnabled(false);
		bIsActive = false;
		return;
	}
	
	FVector NewActorLocation = Spline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	NewActorLocation.Z += ZAxisOffset;
	OverlappedActorPtr->SetActorLocation(NewActorLocation);
	//UE_LOG(LogTemp, Display, TEXT("Distance: %f\tLenght: %f\tSpeed: %f\tDelta Time: %f"), Distance, Spline->GetSplineLength(), Speed, DeltaTime);
}

int AA_Zipline::LastSplinePointIndex() const
{
	return Spline->GetNumberOfSplinePoints() - 1;
}

void AA_Zipline::CreateNewSplineMesh(const uint8 SplineIndex)
{
	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	SplineMeshComponent->SetStaticMesh(SplineMesh);
	SplineMeshComponent->SetMobility(EComponentMobility::Movable);
	SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
	SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepRelativeTransform);

	// Outline scan effect
	SplineMeshComponent->SetRenderCustomDepth(true);
	SplineMeshComponent->SetCustomDepthStencilValue(2);

	const FVector StartPoint = Spline->GetLocationAtSplinePoint(SplineIndex, ESplineCoordinateSpace::Local);
	const FVector StartTangent = Spline->GetTangentAtSplinePoint(SplineIndex, ESplineCoordinateSpace::Local);
	const FVector EndPoint = Spline->GetLocationAtSplinePoint(SplineIndex + 1, ESplineCoordinateSpace::Local);
	const FVector EndTangent = Spline->GetTangentAtSplinePoint(SplineIndex + 1, ESplineCoordinateSpace::Local);

	SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent, true);
	SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SplineMeshComponent->SetForwardAxis(ForwardAxis);
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