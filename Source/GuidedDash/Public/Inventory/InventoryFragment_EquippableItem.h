// Copyright notice

#pragma once

#include "Inventory/InventoryItemDefinition.h"
#include "Templates/SubclassOf.h"

#include "InventoryFragment_EquippableItem.generated.h"

class UEquipmentDefinition;
class UObject;

/*
 *	UInventoryFragment_EquippableItem
 * 
 *	Fragment used to describe a piece of equipment in inventory system
 */
UCLASS()
class GUIDEDDASH_API UInventoryFragment_EquippableItem : public UInventoryItemFragment
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Guided)
	TSubclassOf<UEquipmentDefinition> EquipmentDefinition;
};
