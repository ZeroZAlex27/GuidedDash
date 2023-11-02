// Сopyright notice

#pragma once

#include "ActiveGameplayEffectHandle.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayAbilitySpecHandle.h"
#include "Templates/SubclassOf.h"

#include "GuidedGlobalAbilitySystem.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UGuidedAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;

USTRUCT()
struct FGlobalAppliedAbilityList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UGuidedAbilitySystemComponent>, FGameplayAbilitySpecHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayAbility> Ability, UGuidedAbilitySystemComponent* ASC);
	void RemoveFromASC(UGuidedAbilitySystemComponent* ASC);
	void RemoveFromAll();
};

USTRUCT()
struct FGlobalAppliedEffectList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UGuidedAbilitySystemComponent>, FActiveGameplayEffectHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayEffect> Effect, UGuidedAbilitySystemComponent* ASC);
	void RemoveFromASC(UGuidedAbilitySystemComponent* ASC);
	void RemoveFromAll();
};

/*
 *	UGuidedGlobalAbilitySystem
 * 
 *	WorldSubsystem used to manipulate ASCs
 */
UCLASS()
class GUIDEDDASH_API UGuidedGlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Guided")
	void ApplyAbilityToAll(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Guided")
	void ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Guided")
	void RemoveAbilityFromAll(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Guided")
	void RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect);

	// Register an ASC with global system and apply any active global effects/abilities.
	void RegisterASC(UGuidedAbilitySystemComponent* ASC);

	// Removes an ASC from the global system, along with any active global effects/abilities.
	void UnregisterASC(UGuidedAbilitySystemComponent* ASC);

private:
	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGlobalAppliedAbilityList> AppliedAbilities;

	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FGlobalAppliedEffectList> AppliedEffects;

	UPROPERTY()
	TArray<TObjectPtr<UGuidedAbilitySystemComponent>> RegisteredASCs;
};
