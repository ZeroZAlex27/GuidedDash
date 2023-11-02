// Copyright notice

#pragma once

#include "GameFramework/PlayerController.h"
#include "Teams/TeamAgentInterface.h"

#include "GuidedPlayerController.generated.h"

class APawn;
class AGuidedPlayerState;
class UGuidedAbilitySystemComponent;

/*
 *	AGuidedPlayerController
 *
 *	The base player controller class used by this project.
 */
UCLASS(Config = Game, Meta = (ShortTooltip = "The base player controller class used by this project."))
class GUIDEDDASH_API AGuidedPlayerController : public APlayerController, public ITeamAgentInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Guided|PlayerController")
	AGuidedPlayerState* GetGuidedPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "Guided|PlayerController")
	UGuidedAbilitySystemComponent* GetGuidedAbilitySystemComponent() const;

protected:
	//~AController interface
	virtual void InitPlayerState() override;
	virtual void CleanupPlayerState() override;
	virtual void OnRep_PlayerState() override;
	//~End of AController interface

	//~ITeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ITeamAgentInterface interface

	virtual void AcknowledgePossession(APawn* P) override;
	virtual void OnUnPossess() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Guided|PlayerController")
	void OnAcknowledgePossession(APawn* P);

private:
	UFUNCTION()
	void OnPlayerStateChangedTeam(UObject* TeamAgent, int32 OldTeam, int32 NewTeam);

	void BroadcastOnPlayerStateChanged();

	UPROPERTY()
	FOnTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	TObjectPtr<APlayerState> LastSeenPlayerState;
};
