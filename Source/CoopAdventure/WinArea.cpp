// Fill out your copyright notice in the Description page of Project Settings.


#include "WinArea.h"
#include "Components/BoxComponent.h"
#include "CoopAdventureCharacter.h"

AWinArea::AWinArea()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	WinAreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(WinAreaBox);

	bWin = false;
}

void AWinArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWinArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	if (HasAuthority())	
	{
		if (!bWin)
		{
			TArray<AActor*> Actors;
			WinAreaBox->GetOverlappingActors(Actors, ACoopAdventureCharacter::StaticClass());
			bWin = Actors.Num() == 2;
			if (bWin)
			{
				UE_LOG(LogTemp, Display, TEXT("Win!!!"));
			}
		}
	}
}

