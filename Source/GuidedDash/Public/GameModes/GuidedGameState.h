// Copyright notice

#pragma once

#include "GameFramework/GameStateBase.h"
// Engine
#include "AbilitySystemInterface.h"

#include "GuidedGameState.generated.h"

class UTeamCreationComponent;
class UPlayerSpawningComponent;
class UBotSpawningComponent;
class UAbilitySystemComponent;
class UGuidedAbilitySystemComponent;
class UGamePhaseAbility;
struct FVerbMessage;

/*
 *	AGuidedGameState
 *
 *	The base GameState used by this project.
 */
UCLASS()
class GUIDEDDASH_API AGuidedGameState : public AGameStateBase, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AGuidedGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Gets the ability system component used for game wide things
	UFUNCTION(BlueprintCallable, Category = "Guided|GameState")
	UGuidedAbilitySystemComponent* GetGuidedAbilitySystemComponent() const { return AbilitySystemComponent; }

	// Send a message that all clients will (probably) get
	// (use only for client notifications like eliminations, server join messages, etc... that can handle being lost)
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "Guided|GameState")
	void MulticastMessageToClients(const FVerbMessage Message);

	// Send a message that all clients will be guaranteed to get
	// (use only for client notifications that cannot handle being lost)
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Guided|GameState")
	void MulticastReliableMessageToClients(const FVerbMessage Message);

	void CreateTeamsAndBots();

private:
	// The ability system component subobject for game-wide things (primarily gameplay cues)
	UPROPERTY(VisibleAnywhere, Category = "Guided|GameState")
	TObjectPtr<UGuidedAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Guided|GameState", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTeamCreationComponent> TeamCreationComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Guided|GameState", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerSpawningComponent> PlayerSpawningComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Guided|GameState", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBotSpawningComponent> BotSpawningComp;

	UPROPERTY(EditDefaultsOnly, Category = "Guided|GameState")
	TSubclassOf<UGamePhaseAbility> PhaseAbility;
};
