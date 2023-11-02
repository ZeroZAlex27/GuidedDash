// Copyright notice

#pragma once

#include "AbilitySystem/Abilities/GuidedGameplayAbility.h"

#include "GameplayAbility_Reset.generated.h"

class AActor;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;

USTRUCT(BlueprintType)
struct FPlayerResetMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwnerPlayerState = nullptr;
};

/*
 *  UGameplayAbility_Reset
 *
 *	Gameplay ability used for handling quickly resetting the player back to initial spawn state.
 *	Ability is activated automatically via the "GameplayEvent.RequestReset" ability trigger tag (server only).
 */
UCLASS()
class GUIDEDDASH_API UGameplayAbility_Reset : public UGuidedGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_Reset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
