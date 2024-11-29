// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"

#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "Engine/Texture.h"
#include "Blueprint/UserWidget.h"
#include "Animation/MorphTarget.h"
#include "Components/SkeletalMeshComponent.h"

#include "MyAssetActionUtility.generated.h"

/**
 *
 */
UCLASS()
class COOPADVENTURE_API UMyAssetActionUtility : public UAssetActionUtility
{
	GENERATED_BODY()

public:

	/** 
	 * Rename the selected assets
	 * param SearchPattern - the pattern to search for
   	 * param ReplacePattern - the pattern to replace with
 	 * param SearchCase - should the case be ignored?
	 */
	UFUNCTION(CallInEditor)
	void RenameSelectedAssets(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase);

	/**
	 * Check's if a Texture is a power of two texture
	 */
	UFUNCTION(CallInEditor)
	void CheckPowerOfTwo();

	/**
	 * Add Prefixes to assets (e.g BP_, SM_)
	 */
	UFUNCTION(CallInEditor)
	void AddPrefixes();	

	/**
	 * Cleanup Folder by moving assets
	 *  param ParentFolder - the parent directory
	 */
	UFUNCTION(CallInEditor)
	void CleanupFolder(FString ParentFolder = FString("/Game"));

	/**
	 * Duplicate selected assets
	 *  param NumberOfDuplicates - how many duplicates 
	 *  param bSave - should the duplicates be saved immediately?
	 */
	UFUNCTION(CallInEditor)
	void DuplicateAssets(uint32 NumberOfDuplicates = 1, bool bSave = true);


private:

	const TMap<UClass*, FString> PrefixMap = {
		{UBlueprint::StaticClass(), TEXT("BP_")},
		{UStaticMesh::StaticClass(), TEXT("SM_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialInstance::StaticClass(), TEXT("MI_")},
		{UMaterialFunctionInterface::StaticClass(), TEXT("MF_")},
		{UParticleSystem::StaticClass(), TEXT("PS_")},
		{USoundCue::StaticClass(), TEXT("SC_")},
		{USoundWave::StaticClass(), TEXT("S_")},
		{UTexture::StaticClass(), TEXT("T_")},
		{UTexture2D::StaticClass(), TEXT("T2D_")},
		{UUserWidget::StaticClass(), TEXT("UW_")},
		{UMorphTarget::StaticClass(), TEXT("MT_")},
		{USkeletalMeshComponent::StaticClass(), TEXT("SK_")}
	};

	/**
	 * Check if number is power of two
	 * param NumberToCheck - the number to check
	 */
	bool IsPowerOfTwo(int32 NumberToCheck);

	/**
	 * Print a message to the screen
	 * param Message - the message to print
	 * param Color - the color to use for the Message
	 */
	void PrintToScreen(FString Message, FColor Color);

	/** 
	 * Give Feedback to the user
	 * param Method - the Method that was used
	 * param Counter - how many objects were affected
	*/
	void GiveFeedback(FString Method, uint32 Counter);	
};
