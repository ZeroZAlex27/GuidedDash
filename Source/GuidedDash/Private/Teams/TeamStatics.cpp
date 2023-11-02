// Copyright notice

#include "Teams/TeamStatics.h"
// Engine
#include "Engine/Engine.h"
#include "Engine/World.h"
// Local
#include "Teams/TeamDisplayAsset.h"
#include "Teams/TeamSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TeamStatics)

class UTexture;

void UTeamStatics::FindTeamFromObject(const UObject* Agent, bool& bIsPartOfTeam, int32& TeamId, UTeamDisplayAsset*& DisplayAsset, bool bLogIfNotSet)
{
	bIsPartOfTeam = false;
	TeamId = INDEX_NONE;
	DisplayAsset = nullptr;

	if (UWorld* World = GEngine->GetWorldFromContextObject(Agent, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (UTeamSubsystem* TeamSubsystem = World->GetSubsystem<UTeamSubsystem>())
		{
			TeamId = TeamSubsystem->FindTeamFromObject(Agent);
			if (TeamId != INDEX_NONE)
			{
				bIsPartOfTeam = true;

				DisplayAsset = TeamSubsystem->GetTeamDisplayAsset(TeamId, INDEX_NONE);

				if ((DisplayAsset == nullptr) && bLogIfNotSet)
				{
					UE_LOG(LogTemp, Log, TEXT("FindTeamFromObject(%s) called too early (found team %d but no display asset set yet"), *GetPathNameSafe(Agent), TeamId);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("FindTeamFromObject(%s) failed: Team subsystem does not exist yet"), *GetPathNameSafe(Agent));
		}
	}
}

UTeamDisplayAsset* UTeamStatics::GetTeamDisplayAsset(const UObject* WorldContextObject, int32 TeamId)
{
	UTeamDisplayAsset* Result = nullptr;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (UTeamSubsystem* TeamSubsystem = World->GetSubsystem<UTeamSubsystem>())
		{
			return TeamSubsystem->GetTeamDisplayAsset(TeamId, INDEX_NONE);
		}
	}
	return Result;
}

float UTeamStatics::GetTeamScalarWithFallback(UTeamDisplayAsset* DisplayAsset, FName ParameterName, float DefaultValue)
{
	if (DisplayAsset)
	{
		if (float* pValue = DisplayAsset->ScalarParameters.Find(ParameterName))
		{
			return *pValue;
		}
	}
	return DefaultValue;
}

FLinearColor UTeamStatics::GetTeamColorWithFallback(UTeamDisplayAsset* DisplayAsset, FName ParameterName, FLinearColor DefaultValue)
{
	if (DisplayAsset)
	{
		if (FLinearColor* pColor = DisplayAsset->ColorParameters.Find(ParameterName))
		{
			return *pColor;
		}
	}
	return DefaultValue;
}

UTexture* UTeamStatics::GetTeamTextureWithFallback(UTeamDisplayAsset* DisplayAsset, FName ParameterName, UTexture* DefaultValue)
{
	if (DisplayAsset)
	{
		if (TObjectPtr<UTexture>* pTexture = DisplayAsset->TextureParameters.Find(ParameterName))
		{
			return *pTexture;
		}
	}
	return DefaultValue;
}