// Copyright notice

#include "GameModes/GuidedGameState.h"
// Engine
#include "Engine/World.h"
// Gameplay Message Router
#include "GameFramework/GameplayMessageSubsystem.h"
// Local
#include "GameModes/GameStateComponents/TeamCreationComponent.h"
#include "GameModes/GameStateComponents/PlayerSpawningComponent.h"
#include "GameModes/GameStateComponents/BotSpawningComponent.h"
#include "AbilitySystem/GuidedAbilitySystemComponent.h"
#include "AbilitySystem/Phases/GamePhaseSubsystem.h"
#include "AbilitySystem/Phases/GamePhaseAbility.h"
#include "Messages/VerbMessage.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GuidedGameState)

AGuidedGameState::AGuidedGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UGuidedAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	TeamCreationComp = CreateDefaultSubobject<UTeamCreationComponent>(TEXT("TeamCreationComponent"));
	PlayerSpawningComp = CreateDefaultSubobject<UPlayerSpawningComponent>(TEXT("PlayerSpawningComponent"));
	BotSpawningComp = CreateDefaultSubobject<UBotSpawningComponent>(TEXT("BotSpawningComponent"));
}

UAbilitySystemComponent* AGuidedGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGuidedGameState::MulticastMessageToClients_Implementation(const FVerbMessage Message)
{
	UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
}

void AGuidedGameState::MulticastReliableMessageToClients_Implementation(const FVerbMessage Message)
{
	MulticastMessageToClients_Implementation(Message);
}

void AGuidedGameState::CreateTeamsAndBots()
{
	TeamCreationComp->ServerCreateTeams();
	TeamCreationComp->ServerAssignPlayersToTeams();
	BotSpawningComp->ServerCreateBots();

	if (UGamePhaseSubsystem* PhaseSubsystem = GetWorld()->GetSubsystem<UGamePhaseSubsystem>())
	{
		PhaseSubsystem->StartPhase(PhaseAbility);
	}
}
