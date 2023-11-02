// Copyright notice

#pragma once

#include "Components/ActorComponent.h"

#include "TeamCreationComponent.generated.h"

class ATeamPublicInfo;
class ATeamPrivateInfo;
class AGuidedPlayerState;
class AGameModeBase;
class APlayerController;
class UTeamDisplayAsset;

/*
*	UTeamCreationComponent
*	
*	GameState component used for team creation.
*/
UCLASS(Blueprintable)
class GUIDEDDASH_API UTeamCreationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTeamCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// List of teams to create (id to display asset mapping, the display asset can be left unset if desired)
	UPROPERTY(EditDefaultsOnly, Category = Teams)
	TMap<uint8, TObjectPtr<UTeamDisplayAsset>> TeamsToCreate;

	UPROPERTY(EditDefaultsOnly, Category = Teams)
	TSubclassOf<ATeamPublicInfo> PublicTeamInfoClass;

	UPROPERTY(EditDefaultsOnly, Category = Teams)
	TSubclassOf<ATeamPrivateInfo> PrivateTeamInfoClass;

#if WITH_SERVER_CODE
public:
	virtual void ServerCreateTeams();
	virtual void ServerAssignPlayersToTeams();

	// Sets the team ID for the given player state. Spectator-only player states will be stripped of any team association.
	virtual void ServerChooseTeamForPlayer(AGuidedPlayerState* PS);

private:
	void OnPlayerInitialized(AGameModeBase* GameMode, AController* NewPlayer);
	void ServerCreateTeam(int32 TeamId, UTeamDisplayAsset* DisplayAsset);

	// Returns the Team ID with the fewest active players, or INDEX_NONE if there are no valid teams
	int32 GetLeastPopulatedTeamID() const;
#endif
};
