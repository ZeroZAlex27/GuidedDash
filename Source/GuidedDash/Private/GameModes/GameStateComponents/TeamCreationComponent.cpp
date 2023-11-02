// Copyright notice

#include "GameModes/GameStateComponents/TeamCreationComponent.h"
// Engine
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
// Local
#include "Player/GuidedPlayerState.h"
#include "GameModes/GuidedGameMode.h"
#include "Teams/TeamPublicInfo.h"
#include "Teams/TeamPrivateInfo.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(TeamCreationComponent)

UTeamCreationComponent::UTeamCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PublicTeamInfoClass = ATeamPublicInfo::StaticClass();
	PrivateTeamInfoClass = ATeamPrivateInfo::StaticClass();
}

#if WITH_SERVER_CODE

void UTeamCreationComponent::ServerCreateTeams()
{
	for (const auto& KVP : TeamsToCreate)
	{
		const int32 TeamId = KVP.Key;
		ServerCreateTeam(TeamId, KVP.Value);
	}
}

void UTeamCreationComponent::ServerAssignPlayersToTeams()
{
	// Assign players that already exist to teams
	AGameStateBase* GameState = CastChecked<AGameStateBase>(GetOwner());
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (AGuidedPlayerState* GuidedPS = Cast<AGuidedPlayerState>(PS))
		{
			ServerChooseTeamForPlayer(GuidedPS);
		}
	}

	// Listen for new players logging in
	AGuidedGameMode* GameMode = Cast<AGuidedGameMode>(GameState->AuthorityGameMode);
	check(GameMode);

	GameMode->OnGameModePlayerInitialized.AddUObject(this, &ThisClass::OnPlayerInitialized);
}

void UTeamCreationComponent::ServerChooseTeamForPlayer(AGuidedPlayerState* PS)
{
	if (PS->IsOnlyASpectator())
	{
		PS->SetGenericTeamId(FGenericTeamId::NoTeam);
	}
	else
	{
		const FGenericTeamId TeamID = IntegerToGenericTeamId(GetLeastPopulatedTeamID());
		PS->SetGenericTeamId(TeamID);
	}
}

void UTeamCreationComponent::OnPlayerInitialized(AGameModeBase* GameMode, AController* NewPlayer)
{
	check(NewPlayer);
	check(NewPlayer->PlayerState);
	if (AGuidedPlayerState* GuidedPS = Cast<AGuidedPlayerState>(NewPlayer->PlayerState))
	{
		ServerChooseTeamForPlayer(GuidedPS);
	}
}

void UTeamCreationComponent::ServerCreateTeam(int32 TeamId, UTeamDisplayAsset* DisplayAsset)
{
	check(GetOwner()->HasAuthority());

	// TODO: ensure the team doesn't already exist

	UWorld* World = GetWorld();
	check(World);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATeamPublicInfo* NewTeamPublicInfo = World->SpawnActor<ATeamPublicInfo>(PublicTeamInfoClass, SpawnInfo);
	checkf(NewTeamPublicInfo != nullptr, TEXT("Failed to create public team actor from class %s"), *GetPathNameSafe(*PublicTeamInfoClass));
	NewTeamPublicInfo->SetTeamId(TeamId);
	NewTeamPublicInfo->SetTeamDisplayAsset(DisplayAsset);

	ATeamPrivateInfo* NewTeamPrivateInfo = World->SpawnActor<ATeamPrivateInfo>(PrivateTeamInfoClass, SpawnInfo);
	checkf(NewTeamPrivateInfo != nullptr, TEXT("Failed to create private team actor from class %s"), *GetPathNameSafe(*PrivateTeamInfoClass));
	NewTeamPrivateInfo->SetTeamId(TeamId);
}

int32 UTeamCreationComponent::GetLeastPopulatedTeamID() const
{
	const int32 NumTeams = TeamsToCreate.Num();
	if (NumTeams > 0)
	{
		TMap<int32, uint32> TeamMemberCounts;
		TeamMemberCounts.Reserve(NumTeams);

		for (const auto& KVP : TeamsToCreate)
		{
			const int32 TeamId = KVP.Key;
			TeamMemberCounts.Add(TeamId, 0);
		}

		AGameStateBase* GameState = CastChecked<AGameStateBase>(GetOwner());
		for (APlayerState* PS : GameState->PlayerArray)
		{
			if (AGuidedPlayerState* GuidedPS = Cast<AGuidedPlayerState>(PS))
			{
				const int32 PlayerTeamID = GuidedPS->GetTeamId();

				if ((PlayerTeamID != INDEX_NONE) && !GuidedPS->IsInactive())	// do not count unassigned or disconnected players
				{
					check(TeamMemberCounts.Contains(PlayerTeamID))
						TeamMemberCounts[PlayerTeamID] += 1;
				}
			}
		}

		// Sort by lowest team population, then by team ID
		int32 BestTeamId = INDEX_NONE;
		uint32 BestPlayerCount = TNumericLimits<uint32>::Max();
		for (const auto& KVP : TeamMemberCounts)
		{
			const int32 TestTeamId = KVP.Key;
			const uint32 TestTeamPlayerCount = KVP.Value;

			if (TestTeamPlayerCount < BestPlayerCount)
			{
				BestTeamId = TestTeamId;
				BestPlayerCount = TestTeamPlayerCount;
			}
			else if (TestTeamPlayerCount == BestPlayerCount)
			{
				if ((TestTeamId < BestTeamId) || (BestTeamId == INDEX_NONE))
				{
					BestTeamId = TestTeamId;
					BestPlayerCount = TestTeamPlayerCount;
				}
			}
		}

		return BestTeamId;
	}

	return INDEX_NONE;
}
#endif	// WITH_SERVER_CODE