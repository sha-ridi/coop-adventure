// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableKey.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"
#include "CoopAdventureCharacter.h"
#include "Components/AudioComponent.h"

// Sets default values
ACollectableKey::ACollectableKey()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);
	Mesh->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComp->SetupAttachment(RootComp);
	CapsuleComp->SetIsReplicated(true);
	CapsuleComp->SetCollisionProfileName(FName(TEXT("OverlapAllDynamic")));
	CapsuleComp->SetCapsuleSize(100.f, 150.f);

	CollectAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("Collect Audio"));
	CollectAudio->SetupAttachment(RootComp);
	CollectAudio->SetAutoActivate(false);

	RotationSpeed = 100.f;

}

void ACollectableKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACollectableKey, bCollected);
}

void ACollectableKey::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
	{
		// Rotate Static Mesh
		Mesh->AddRelativeRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));

		TArray<AActor *> Actors;
		CapsuleComp->GetOverlappingActors(Actors, ACoopAdventureCharacter::StaticClass());
		if (!Actors.IsEmpty())
		{
			// A Player Character Is Overlapping The Capsule
			if (!bCollected)
			{
				bCollected = true;
				OnRep_bCollected();
			}			
		}
	}
}

void ACollectableKey::OnRep_bCollected()
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Display, TEXT("OnRep_bCollected Server"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("OnRep_bCollected Client"));
	}

	Mesh->SetVisibility(!bCollected);

	CollectAudio->Play();
}

