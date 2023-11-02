// Copyright notice

#pragma once

#include "GameFramework/PlayerState.h"
// Gameplay Ability System
#include "AbilitySystemInterface.h"
// Local
#include "Teams/TeamAgentInterface.h"

#include "GuidedPlayerState.generated.h"

class AGuidedPlayerController;
class APlayerState;
class UAbilitySystemComponent;
class UGuidedAbilitySystemComponent;
class UPawnData;
class UHealthSet;
class UCombatSet;
struct FGameplayTag;
struct FGameplayTagStackContainer;
struct FVerbMessage;

/*
 *	AGuidedPlayerState
 *
 *	Base player state class used by this project.
 */
UCLASS(Config = Game)
class GUIDEDDASH_API AGuidedPlayerState : public APlayerState, public IAbilitySystemInterface, public ITeamAgentInterface
{
	GENERATED_BODY()
	
public:
	AGuidedPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Guided|PlayerState")
	AGuidedPlayerController* GetGuidedPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Guided|PlayerState")
	UGuidedAbilitySystemComponent* GetGuidedAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UPawnData* InPawnData);

	//~ITeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ITeamAgentInterface interface

	// Returns the Squad ID of the squad the player belongs to.
	UFUNCTION(BlueprintCallable)
		int32 GetSquadId() const
	{
		return MySquadID;
	}

	// Returns the Team ID of the team the player belongs to.
	UFUNCTION(BlueprintCallable)
		int32 GetTeamId() const
	{
		return GenericTeamIdToInteger(MyTeamID);
	}

	void SetSquadID(int32 NewSquadID);

	// Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Teams)
	void AddStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Teams)
	void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);

	// Returns the stack count of the specified tag (or 0 if the tag is not present)
	UFUNCTION(BlueprintCallable, Category = Teams)
	int32 GetStatTagStackCount(FGameplayTag Tag) const;

	// Returns true if there is at least one stack of the specified tag
	UFUNCTION(BlueprintCallable, Category = Teams)
	bool HasStatTag(FGameplayTag Tag) const;

	// Send a message to just this player
	// (use only for client notifications like accolades, quest toasts, etc... that can handle being occasionally lost)
	UFUNCTION(Client, Unreliable, BlueprintCallable, Category = "Lyra|PlayerState")
	void ClientBroadcastMessage(const FVerbMessage Message);

protected:
	UFUNCTION()
	void OnRep_PawnData();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UPawnData> PawnData;

private:
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Guided|PlayerState")
	TObjectPtr<UGuidedAbilitySystemComponent> AbilitySystemComponent;

	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UHealthSet> HealthSet;
	// Combat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class UCombatSet> CombatSet;

	UPROPERTY()
	FOnTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY(ReplicatedUsing = OnRep_MyTeamID)
	FGenericTeamId MyTeamID;

	UPROPERTY(ReplicatedUsing = OnRep_MySquadID)
	int32 MySquadID;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer StatTags;

private:
	UFUNCTION()
	void OnRep_MyTeamID(FGenericTeamId OldTeamID);

	UFUNCTION()
	void OnRep_MySquadID();
};
