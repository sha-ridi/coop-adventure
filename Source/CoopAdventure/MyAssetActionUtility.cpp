// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAssetActionUtility.h"
#include "EditorUtilityLibrary.h"

#pragma region RenameSelectedAssets

void UMyAssetActionUtility::RenameSelectedAssets(FString SearchPattern, FString ReplacePattern, ESearchCase::Type SearchCase)
{
    // Check if something needs to be done
    if (SearchPattern.IsEmpty() || SearchPattern.Equals(ReplacePattern, SearchCase))
    {
        return;
    }

    // Get Selected Objects
    TArray<UObject* > SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();

    uint32 Counter = 0;

    // Check each Asset if it needs to be renamed
    for (UObject* SelectedObject : SelectedObjects)
    {
        if (ensure(SelectedObject))
        {
            FString AssetName = SelectedObject->GetName();
            if (AssetName.Contains(SearchPattern, SearchCase))
            {
                FString NewName = AssetName.Replace(*SearchPattern, *ReplacePattern, SearchCase);
                UEditorUtilityLibrary::RenameAsset(SelectedObject, NewName);
                ++Counter;
            }
        }        
    }

    GiveFeedback(TEXT("Renamed"), Counter);
}

#pragma endregion

#pragma region Helper

void UMyAssetActionUtility::PrintToScreen(FString Message, FColor Color)
{
    if (ensure(GEngine))
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, Color, Message);
        UE_LOG(LogTemp, Display, TEXT("%s"), *Message);
    }
}

void UMyAssetActionUtility::GiveFeedback(FString Method, uint32 Counter)
{
    FString Message = FString("No matching files found");
    FColor Color = Counter > 0 ? FColor::Green : FColor::Red;
    if (Counter > 0)
    {
        Message = Method.AppendChar(' ');
        Message.AppendInt(Counter);
        Message.Append(Counter == 1 ? TEXT(" file.") : TEXT(" files."));
    }
    PrintToScreen(Message, Color);
}

#pragma endregion