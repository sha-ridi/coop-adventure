// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlate.h"
#include "Transporter.h"

// Sets default values
APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = true;

	bActivated = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trigger Mesh"));
	TriggerMesh->SetupAttachment(RootComp);
	TriggerMesh->SetIsReplicated(true);

	auto TriggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/BasicShapes/Cylinder"));

	if (TriggerMeshAsset.Succeeded())
	{
		TriggerMesh->SetStaticMesh(TriggerMeshAsset.Object);
		TriggerMesh->SetRelativeScale3D(FVector(3.3f, 3.3f, 0.2f));
		TriggerMesh->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Meshes/Sphere"));

	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeScale3D(FVector(3.5f, 3.5f, 0.5f));
		Mesh->SetRelativeLocation(FVector(0.f, 0.f, -7.8f));
	}

	Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));
	Transporter->MoveTime = 0.35f;
	Transporter->bOwnerIsTriggerActor = true;
}

// Called when the game starts or when spawned
void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	SetReplicates(true);
	SetReplicatingMovement(true);

	TriggerMesh->SetVisibility(false);
	TriggerMesh->SetCollisionProfileName(FName(TEXT("OverlapAll")));
	
	FVector Point1 = GetActorLocation();
	FVector Point2 = GetActorLocation() + FVector(0, 0, -12.f);
	Transporter->SetPoints(Point1, Point2);
}

// Called every frame
void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		TArray<AActor *> Actors;
		AActor * TriggerActor = 0;
		TriggerMesh->GetOverlappingActors(Actors);
		for (AActor* Actor : Actors)
		{
			if (Actor->ActorHasTag("TriggerActor"))
			{
				TriggerActor = Actor;
				break;
			}			
		}

		if (TriggerActor)
		{
			if (!bActivated)
			{
				bActivated = true;
				OnActivated.Broadcast();
				// UE_LOG(LogTemp, Display, TEXT("Activated"));
			}
		}
		else
		{
			if (bActivated)
			{
				bActivated = false;
				OnDeactivated.Broadcast();
				// UE_LOG(LogTemp, Display, TEXT("Deactivated"));
			}
		}
	}
}

