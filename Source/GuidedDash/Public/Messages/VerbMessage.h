// Copyright notice

#pragma once

#include "UObject/Object.h"
#include "Containers/UnrealString.h"
#include "GameplayTagContainer.h"

#include "VerbMessage.generated.h"

/*
*	FVerbMessage
* 
*	Represents a generic message of the form Instigator Verb Target (in Context, with Magnitude)
*/
USTRUCT(BlueprintType)
struct FVerbMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTag Verb;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<UObject> Instigator = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	TObjectPtr<UObject> Target = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTagContainer InstigatorTags;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTagContainer TargetTags;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	FGameplayTagContainer ContextTags;

	UPROPERTY(BlueprintReadWrite, Category = Gameplay)
	double Magnitude = 1.0;

	// Returns a debug string representation of this message
	GUIDEDDASH_API FString ToString() const;
};