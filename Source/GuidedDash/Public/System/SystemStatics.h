// Copyright notice

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/SoftObjectPtr.h"

#include "SystemStatics.generated.h"

template <typename T> class TSubclassOf;

class AActor;
class UObject;
class UActorComponent;

/*
*	USystemStatics
* 
*	BlueprintFunctionLibrary with functions used by this project
*/
UCLASS()
class GUIDEDDASH_API USystemStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Returns the soft object reference associated with a Primary Asset Id, this works even if the asset is not loaded
	UFUNCTION(BlueprintPure, Category = "AssetManager", meta = (DeterminesOutputType = ExpectedAssetType))
	static TSoftObjectPtr<UObject> GetTypedSoftObjectReferenceFromPrimaryAssetId(FPrimaryAssetId PrimaryAssetId, TSubclassOf<UObject> ExpectedAssetType);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Guided", meta = (WorldContext = "WorldContextObject"))
	static void PlayNextGame(const UObject* WorldContextObject);

	// Sets ParameterName to ParameterValue on all sections of all mesh components found on the TargetActor
	UFUNCTION(BlueprintCallable, Category = "Rendering|Material", meta = (DefaultToSelf = "TargetActor"))
	static void SetScalarParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName, const float ParameterValue, bool bIncludeChildActors = true);

	// Sets ParameterName to ParameterValue on all sections of all mesh components found on the TargetActor
	UFUNCTION(BlueprintCallable, Category = "Rendering|Material", meta = (DefaultToSelf = "TargetActor"))
	static void SetVectorParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName, const FVector ParameterValue, bool bIncludeChildActors = true);

	// Sets ParameterName to ParameterValue on all sections of all mesh components found on the TargetActor
	UFUNCTION(BlueprintCallable, Category = "Rendering|Material", meta = (DefaultToSelf = "TargetActor"))
	static void SetColorParameterValueOnAllMeshComponents(AActor* TargetActor, const FName ParameterName, const FLinearColor ParameterValue, bool bIncludeChildActors = true);

	// Gets all the components that inherit from the given class
	UFUNCTION(BlueprintCallable, Category = "Actor", meta = (DefaultToSelf = "TargetActor", ComponentClass = "/Script/Engine.ActorComponent", DeterminesOutputType = "ComponentClass"))
	static TArray<UActorComponent*> FindComponentsByClass(AActor* TargetActor, TSubclassOf<UActorComponent> ComponentClass, bool bIncludeChildActors = true);
};
