// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
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
	 *  Rename the selected assets
	 * param SearchPattern - the pattern to search for
   	 * param ReplacePattern - the pattern to replace with
 	 * param SearchCase - should the case be ignored?
	 */
	UFUNCTION(CallInEditor)
	void RenameSelectedAssets(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase);

private:
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
