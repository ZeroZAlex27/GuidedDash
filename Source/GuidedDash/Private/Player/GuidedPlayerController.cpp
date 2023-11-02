// Copyright notice

#include "Player/GuidedPlayerController.h"
// Engine
#include "GameFramework/Pawn.h"
// Gameplay Ability System
#include "AbilitySystemGlobals.h"
// Local
#include "Player/GuidedPlayerState.h"
#include "UI/GuidedHUD.h"
#include "AbilitySystem/GuidedAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GuidedPlayerController)

AGuidedPlayerState* AGuidedPlayerController::GetGuidedPlayerState() const
{
	return CastChecked<AGuidedPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UGuidedAbilitySystemComponent* AGuidedPlayerController::GetGuidedAbilitySystemComponent() const
{
	const AGuidedPlayerState* PS = GetGuidedPlayerState();
	return (PS ? PS->GetGuidedAbilitySystemComponent() : nullptr);
}

void AGuidedPlayerController::InitPlayerState()
{
	Super::InitPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AGuidedPlayerController::CleanupPlayerState()
{
	Super::CleanupPlayerState();
	BroadcastOnPlayerStateChanged();
}

void AGuidedPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	BroadcastOnPlayerStateChanged();
}

void AGuidedPlayerController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	UE_LOG(LogTemp, Error, TEXT("You can't set the team ID on a player controller (%s); it's driven by the associated player state"), *GetPathNameSafe(this));
}

FGenericTeamId AGuidedPlayerController::GetGenericTeamId() const
{
	if (const ITeamAgentInterface* PSWithTeamInterface = Cast<ITeamAgentInterface>(PlayerState))
	{
		return PSWithTeamInterface->GetGenericTeamId();
	}
	return FGenericTeamId::NoTeam;
}

FOnTeamIndexChangedDelegate* AGuidedPlayerController::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void AGuidedPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UGuidedAbilitySystemComponent* ASC = GetGuidedAbilitySystemComponent())
	{
		ASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AGuidedPlayerController::OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam)
{
	ConditionalBroadcastTeamChanged(this, IntegerToGenericTeamId(OldTeam), IntegerToGenericTeamId(NewTeam));
}

void AGuidedPlayerController::BroadcastOnPlayerStateChanged()
{
	// Unbind from the old player state, if any
	FGenericTeamId OldTeamID = FGenericTeamId::NoTeam;
	if (LastSeenPlayerState != nullptr)
	{
		if (ITeamAgentInterface* PlayerStateTeamInterface = Cast<ITeamAgentInterface>(LastSeenPlayerState))
		{
			OldTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().RemoveAll(this);
		}
	}

	// Bind to the new player state, if any
	FGenericTeamId NewTeamID = FGenericTeamId::NoTeam;
	if (PlayerState != nullptr)
	{
		if (ITeamAgentInterface* PlayerStateTeamInterface = Cast<ITeamAgentInterface>(PlayerState))
		{
			NewTeamID = PlayerStateTeamInterface->GetGenericTeamId();
			PlayerStateTeamInterface->GetTeamChangedDelegateChecked().AddDynamic(this, &ThisClass::OnPlayerStateChangedTeam);
		}
	}

	// Broadcast the team change (if it really has)
	ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);

	LastSeenPlayerState = PlayerState;
}

void AGuidedPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	// Only create a HUD for local player
	if (IsLocalPlayerController())
	{
		if (AGuidedHUD* HUD = GetHUD<AGuidedHUD>())
		{
			HUD->CreateHUD();
		}
	}

	OnAcknowledgePossession(P);
}

void AGuidedPlayerController::OnUnPossess()
{
	// Make sure the pawn that is being unpossessed doesn't remain our ASC's avatar actor
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}

	Super::OnUnPossess();
}
