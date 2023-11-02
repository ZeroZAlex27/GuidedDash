// Copyright notice

#pragma once

#include "AbilitySystem/Abilities/GuidedGameplayAbility.h"

#include "GameplayAbility_FromEquipment.generated.h"

class UEquipmentInstance;
class UInventoryItemInstance;

/*
 *	UGameplayAbility_FromEquipment
 *
 *	An ability granted by and associated with an equipment instance
 */
UCLASS()
class GUIDEDDASH_API UGameplayAbility_FromEquipment : public UGuidedGameplayAbility
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Guided|Ability")
	UEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "Guided|Ability")
	UInventoryItemInstance* GetAssociatedItem() const;

	UFUNCTION(BlueprintCallable, Category = "Guided|Ability")
	AActor* GetFirstActorFromAssociatedEquipment() const;
};
