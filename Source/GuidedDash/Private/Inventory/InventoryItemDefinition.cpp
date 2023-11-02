// Copyright notice

#include "Inventory/InventoryItemDefinition.h"
// Engine
#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(InventoryItemDefinition)

//////////////////////////////////////////////////////////////////////
// UInventoryItemDefinition

const UInventoryItemFragment* UInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////
// UInventoryItemDefinition

const UInventoryItemFragment* UInventoryFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UInventoryItemDefinition> ItemDef, TSubclassOf<UInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
