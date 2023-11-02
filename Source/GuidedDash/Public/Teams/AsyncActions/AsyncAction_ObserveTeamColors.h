// Copyright notice

#pragma once

#include "Engine/CancellableAsyncAction.h"
#include "UObject/WeakInterfacePtr.h"

#include "AsyncAction_ObserveTeamColors.generated.h"

class ITeamAgentInterface;
class UTeamDisplayAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FTeamColorObservedAsyncDelegate, bool, bTeamSet, int32, TeamId, const UTeamDisplayAsset*, DisplayAsset);

/*
*	UAsyncAction_ObserveTeamColors
* 
*	Watches for team color changes in the specified object
*/
UCLASS()
class GUIDEDDASH_API UAsyncAction_ObserveTeamColors : public UCancellableAsyncAction
{
	GENERATED_BODY()
	
public:
	// Watches for team changes on the specified team agent
	//  - It will will fire once immediately to give the current team assignment
	//  - For anything that can ever belong to a team (implements ILyraTeamAgentInterface),
	//    it will also listen for team assignment changes in the future
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Keywords = "Watch"))
	static UAsyncAction_ObserveTeamColors* ObserveTeamColors(UObject* TeamAgent);

	//~UBlueprintAsyncActionBase interface
	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;
	//~End of UBlueprintAsyncActionBase interface

public:
	// Called when the team is set or changed
	UPROPERTY(BlueprintAssignable)
	FTeamColorObservedAsyncDelegate OnTeamChanged;

private:
	void BroadcastChange(int32 NewTeam, const UTeamDisplayAsset* DisplayAsset);

	UFUNCTION()
	void OnWatchedAgentChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

	UFUNCTION()
	void OnDisplayAssetChanged(const UTeamDisplayAsset* DisplayAsset);

	TWeakInterfacePtr<ITeamAgentInterface> TeamInterfacePtr;
	TWeakObjectPtr<UObject> TeamInterfaceObj;

	int32 LastBroadcastTeamId = INDEX_NONE;
};
