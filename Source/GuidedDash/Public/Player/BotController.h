// Copyright notice

#pragma once

#include "AIController.h"
#include "Teams/TeamAgentInterface.h"

#include "BotController.generated.h"

class UAIPerceptionComponent;

namespace ETeamAttitude { enum Type : int; }

/*
 *	ABotController
 *
 *	The controller class used by player bots in this project.
 */
UCLASS(Blueprintable)
class GUIDEDDASH_API ABotController : public AAIController, public ITeamAgentInterface
{
	GENERATED_BODY()
	
public:
	ABotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~ITeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//~End of ITeamAgentInterface interface

	// Attempts to restart this controller (e.g., to respawn it)
	void ServerRestartController();

	// Update Team Attitude for the AI
	UFUNCTION(BlueprintCallable, Category = "AI Controller")
	void UpdateTeamAttitude(UAIPerceptionComponent* AIPerception);

	virtual void OnUnPossess() override;

private:
	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

private:
	void BroadcastOnPlayerStateChanged();

protected:
	//~AController interface
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

private:
	UPROPERTY()
	FOnTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;
};
