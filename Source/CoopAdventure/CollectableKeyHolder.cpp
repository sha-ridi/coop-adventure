// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableKeyHolder.h"

// Sets default values
ACollectableKeyHolder::ACollectableKeyHolder()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);
	Mesh->SetCollisionProfileName(FName(TEXT("BlockAllDynamic")));

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key Mesh"));
	KeyMesh->SetupAttachment(RootComp);
	KeyMesh->SetIsReplicated(true);
	KeyMesh->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));

	KeyMeshRotationSpeed = 100.f;

}

void ACollectableKeyHolder::BeginPlay()
{
	Super::BeginPlay();
	
	KeyMesh->SetVisibility(false);
}

void ACollectableKeyHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		KeyMesh->AddRelativeRotation(FRotator(0.f, KeyMeshRotationSpeed * DeltaTime, 0.f));
	}

}

void ACollectableKeyHolder::ActivateKeyMesh()
{
	KeyMesh->SetVisibility(true);
}

