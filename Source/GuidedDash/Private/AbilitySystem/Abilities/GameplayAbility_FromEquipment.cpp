// Copyright notice

#include "AbilitySystem/Abilities/GameplayAbility_FromEquipment.h"
// Local
#include "Equipment/EquipmentInstance.h"
#include "Inventory/InventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameplayAbility_FromEquipment)

UEquipmentInstance* UGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

UInventoryItemInstance* UGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}

AActor* UGameplayAbility_FromEquipment::GetFirstActorFromAssociatedEquipment() const
{
	if (UEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		if (Equipment->GetSpawnedActors()[0] != nullptr)
		{
			return Equipment->GetSpawnedActors()[0];
		}
	}
	return nullptr;
}
