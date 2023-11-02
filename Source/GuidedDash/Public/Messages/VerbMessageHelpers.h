// Copyright notice

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "VerbMessageHelpers.generated.h"

struct FGameplayCueParameters;
struct FVerbMessage;

class APlayerController;
class APlayerState;

/*
 *	UVerbMessageHelpers
 * 
 *	BlueprintFunctionLibrary that can be useful with VerbMessage.
 */
UCLASS()
class GUIDEDDASH_API UVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Message")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Message")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Message")
	static FGameplayCueParameters VerbMessageToCueParameters(const FVerbMessage& Message);

	UFUNCTION(BlueprintCallable, Category = "Message")
	static FVerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);
};
