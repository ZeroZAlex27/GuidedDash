// Copyright notice

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "TeamStatics.generated.h"

class UTeamDisplayAsset;
class UTexture;

/*
*	UTeamStatics
* 
*	A subsystem for easy access to team information for team-based actors (e.g., pawns or player states) 
*/
UCLASS()
class GUIDEDDASH_API UTeamStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Returns the team this object belongs to, or INDEX_NONE if it is not part of a team
	UFUNCTION(BlueprintCallable, Category = Teams, meta = (Keywords = "GetTeamFromObject", DefaultToSelf = "Agent", AdvancedDisplay = "bLogIfNotSet"))
	static void FindTeamFromObject(const UObject* Agent, bool& bIsPartOfTeam, int32& TeamId, UTeamDisplayAsset*& DisplayAsset, bool bLogIfNotSet = false);

	UFUNCTION(BlueprintCallable, Category = Teams, meta = (WorldContext = "WorldContextObject"))
	static UTeamDisplayAsset* GetTeamDisplayAsset(const UObject* WorldContextObject, int32 TeamId);

	UFUNCTION(BlueprintCallable, Category = Teams)
	static float GetTeamScalarWithFallback(UTeamDisplayAsset* DisplayAsset, FName ParameterName, float DefaultValue);

	UFUNCTION(BlueprintCallable, Category = Teams)
	static FLinearColor GetTeamColorWithFallback(UTeamDisplayAsset* DisplayAsset, FName ParameterName, FLinearColor DefaultValue);

	UFUNCTION(BlueprintCallable, Category = Teams)
	static UTexture* GetTeamTextureWithFallback(UTeamDisplayAsset* DisplayAsset, FName ParameterName, UTexture* DefaultValue);
};
