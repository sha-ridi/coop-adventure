// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableActor.h"
#include "Components/ArrowComponent.h"
#include "Transporter.h"

AMovableActor::AMovableActor()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);

	Point1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point 1"));
	Point1->SetupAttachment(RootComp);
	Point1->SetRelativeLocation(FVector::Zero());

	Point2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point 2"));
	Point2->SetupAttachment(RootComp);
	Point2->SetRelativeLocation(FVector(0.f, 0.f, 300.f));

	Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
}

void AMovableActor::BeginPlay()
{
	Super::BeginPlay();
	
	FVector StartPoint = GetActorLocation() + Point1->GetRelativeLocation();
	FVector EndPoint = GetActorLocation() + Point2->GetRelativeLocation();
	Transporter->SetPoints(StartPoint, EndPoint);
}

void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

