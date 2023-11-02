// Copyright notice

#pragma once

#include "AbilitySystem/Abilities/GuidedGameplayAbility.h"

#include "GameplayAbility_Jump.generated.h"

struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

/*
 *	UGameplayAbility_Jump
 *
 *	Gameplay ability used for character jumping.
 */
UCLASS(Abstract)
class GUIDEDDASH_API UGameplayAbility_Jump : public UGuidedGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category = "Guided|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Guided|Ability")
	void CharacterJumpStop();
};
