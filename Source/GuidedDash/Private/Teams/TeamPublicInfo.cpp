// Copyright notice

#include "Teams/TeamPublicInfo.h"
// Engine
#include "Net/UnrealNetwork.h"
// Local
#include "Teams/TeamInfoBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TeamPublicInfo)

void ATeamPublicInfo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, TeamDisplayAsset, COND_InitialOnly);
}

void ATeamPublicInfo::SetTeamDisplayAsset(TObjectPtr<UTeamDisplayAsset> NewDisplayAsset)
{
	check(HasAuthority());
	check(TeamDisplayAsset == nullptr);

	TeamDisplayAsset = NewDisplayAsset;

	TryRegisterWithTeamSubsystem();
}

void ATeamPublicInfo::OnRep_TeamDisplayAsset()
{
	TryRegisterWithTeamSubsystem();
}