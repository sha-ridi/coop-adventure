// Fill out your copyright notice in the Description page of Project Settings.


#include "Transporter.h"
#include "PressurePlate.h"
#include "CollectableKey.h"

UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	MoveTime = 3.f;
	ActivatedTriggerCount = 0;
	bArePointsSet = false;
	StartPoint = FVector(0);
	EndPoint = FVector(0);
}


void UTransporter::BeginPlay()
{
	Super::BeginPlay();

	if (bOwnerIsTriggerActor)
	{
		TriggerActors.Add(GetOwner());
	}
	
	for (AActor * TriggerActor : TriggerActors)
	{
		APressurePlate* PreassurePlate = Cast<APressurePlate>(TriggerActor);
		if (PreassurePlate)
		{
			PreassurePlate->OnActivated.AddDynamic(this, &UTransporter::OnTriggerActorActivated);
			PreassurePlate->OnDeactivated.AddDynamic(this, &UTransporter::OnTriggerActorDeactivated);
			continue;
		}
		ACollectableKey* CollectableKey = Cast<ACollectableKey>(TriggerActor);
		if (CollectableKey)
		{
			CollectableKey->OnCollected.AddDynamic(this, &UTransporter::OnTriggerActorActivated);
		}
	}
}


void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TriggerActors.Num() > 0)
	{
		bAllTriggerActorsTriggered = ActivatedTriggerCount >= TriggerActors.Num();
	}

	AActor* Owner = GetOwner();
	if (Owner && Owner->HasAuthority() && bArePointsSet)
	{
		FVector CurrentLocation = Owner->GetActorLocation();
		float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;

		FVector TargetLocation = bAllTriggerActorsTriggered ? EndPoint : StartPoint;
		if (!CurrentLocation.Equals(TargetLocation))
		{
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
			Owner->SetActorLocation(NewLocation);
		}
	}
}

void UTransporter::SetPoints(FVector Point1, FVector Point2)
{
	if (Point1.Equals(Point2)) return;

	StartPoint = Point1;
	EndPoint = Point2;
	bArePointsSet = true;
}

void UTransporter::OnTriggerActorActivated()
{
	ActivatedTriggerCount++;
}

void UTransporter::OnTriggerActorDeactivated()
{
	ActivatedTriggerCount--;
}




