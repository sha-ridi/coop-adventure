// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectableKey.generated.h"

class UCapsuleComponent;
class UAudioComponent;
class ACollectableKeyHolder;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCollectableKeyOnCollected);

UCLASS()
class COOPADVENTURE_API ACollectableKey : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableKey();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;	

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(ReplicatedUsing = OnRep_bCollected, BlueprintReadWrite, VisibleAnywhere)
	bool bCollected;

	UFUNCTION(BlueprintCallable)
	void OnRep_bCollected();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RotationSpeed;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UAudioComponent* CollectAudio;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ACollectableKeyHolder* KeyHolder;

	UPROPERTY(BlueprintAssignable)
	FCollectableKeyOnCollected OnCollected;

};
