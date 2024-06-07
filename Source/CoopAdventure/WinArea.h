// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinArea.generated.h"

class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWinAreaOnWin);

UCLASS()
class COOPADVENTURE_API AWinArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AWinArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* WinAreaBox;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bWin;	

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCWin();

	UPROPERTY(BlueprintAssignable)
	FWinAreaOnWin OnWin;
	
};
