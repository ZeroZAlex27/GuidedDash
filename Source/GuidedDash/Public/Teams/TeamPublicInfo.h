// Copyright notice

#pragma once

#include "Teams/TeamInfoBase.h"

#include "TeamPublicInfo.generated.h"

class UTeamCreationComponent;
class UTeamDisplayAsset;

/*
 *	ATeamPublicInfo
 * 
 *	Used to store public info of a team
 */
UCLASS()
class GUIDEDDASH_API ATeamPublicInfo : public ATeamInfoBase
{
	GENERATED_BODY()
	
	friend UTeamCreationComponent;

public:
	UTeamDisplayAsset* GetTeamDisplayAsset() const { return TeamDisplayAsset; }

private:
	UFUNCTION()
	void OnRep_TeamDisplayAsset();

	void SetTeamDisplayAsset(TObjectPtr<UTeamDisplayAsset> NewDisplayAsset);

private:
	UPROPERTY(ReplicatedUsing = OnRep_TeamDisplayAsset)
	TObjectPtr<UTeamDisplayAsset> TeamDisplayAsset;
};
