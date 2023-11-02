// Copyright notice

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "InventoryItemDefinition.generated.h"

template <typename T> class TSubclassOf;

class UInventoryItemInstance;

/*
*	UInventoryItemFragment
* 
*	Represents a fragment of an item definition
*/
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class GUIDEDDASH_API UInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UInventoryItemInstance* Instance) const {}
};

/*
 *	UInventoryItemDefinition
 * 
 *	The base Definition used in inventory system for this project
 */
UCLASS(Blueprintable, Const, Abstract)
class GUIDEDDASH_API UInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, Instanced)
	TArray<TObjectPtr<UInventoryItemFragment>> Fragments;

public:
	const UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass) const;
};

// TODO: Make into a subsystem instead?
UCLASS()
class UInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta = (DeterminesOutputType = FragmentClass))
	static const UInventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<UInventoryItemDefinition> ItemDef, TSubclassOf<UInventoryItemFragment> FragmentClass);
};
