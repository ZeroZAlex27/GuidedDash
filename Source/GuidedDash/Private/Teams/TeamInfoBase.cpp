// Fill out your copyright notice in the Description page of Project Settings.


#include "Teams/TeamInfoBase.h"
// Engine
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
// Local
#include "Teams/TeamSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TeamInfoBase)

ATeamInfoBase::ATeamInfoBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TeamId(INDEX_NONE)
{
	bReplicates = true;
	bAlwaysRelevant = true;
	NetPriority = 3.0f;
	SetReplicatingMovement(false);
}

void ATeamInfoBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, TeamTags);
	DOREPLIFETIME_CONDITION(ThisClass, TeamId, COND_InitialOnly);
}

void ATeamInfoBase::BeginPlay()
{
	Super::BeginPlay();

	TryRegisterWithTeamSubsystem();
}

void ATeamInfoBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (TeamId != INDEX_NONE)
	{
		UTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UTeamSubsystem>();
		if (TeamSubsystem)
		{
			// EndPlay can happen at weird times where the subsystem has already been destroyed
			TeamSubsystem->UnregisterTeamInfo(this);
		}
	}

	Super::EndPlay(EndPlayReason);
}

void ATeamInfoBase::RegisterWithTeamSubsystem(UTeamSubsystem* Subsystem)
{
	Subsystem->RegisterTeamInfo(this);
}

void ATeamInfoBase::TryRegisterWithTeamSubsystem()
{
	if (TeamId != INDEX_NONE)
	{
		if (UTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UTeamSubsystem>())
		{
			RegisterWithTeamSubsystem(TeamSubsystem);
		}
	}
}

void ATeamInfoBase::SetTeamId(int32 NewTeamId)
{
	check(HasAuthority());
	check(TeamId == INDEX_NONE);
	check(NewTeamId != INDEX_NONE);

	TeamId = NewTeamId;

	TryRegisterWithTeamSubsystem();
}

void ATeamInfoBase::OnRep_TeamId()
{
	TryRegisterWithTeamSubsystem();
}