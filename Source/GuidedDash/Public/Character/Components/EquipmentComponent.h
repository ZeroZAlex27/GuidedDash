// Copyright notice

#pragma once

// Engine
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
// Local
#include "AbilitySystem/AbilitySet.h"

#include "EquipmentComponent.generated.h"

class UGuidedAbilitySystemComponent;
class UEquipmentDefinition;
class UEquipmentInstance;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/*
*	FAppliedEquipmentEntry
* 
*	A single piece of applied equipment 
*/
USTRUCT(BlueprintType)
struct FAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FEquipmentList;
	friend UEquipmentComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UEquipmentInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FAbilitySet_GrantedHandles GrantedHandles;
};

/*
*	FEquipmentList
* 
*	List of applied equipment 
*/
USTRUCT(BlueprintType)
struct FEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FEquipmentList()
	: OwnerComponent(nullptr)
	{
	}

	FEquipmentList(UActorComponent* InOwnerComponent)
	: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FAppliedEquipmentEntry, FEquipmentList>(Entries, DeltaParms, *this);
	}

	UEquipmentInstance* AddEntry(TSubclassOf<UEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UEquipmentInstance* Instance);

private:
	UGuidedAbilitySystemComponent* GetGuidedAbilitySystemComponent() const;

	friend UEquipmentComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FEquipmentList> : public TStructOpsTypeTraitsBase2<FEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};

/*
*	UEquipmentComponent 
* 
*	Manages equipment applied to a pawn
* 
*	Note: Not optimized. Requires more improvements for further work.
*/
UCLASS(BlueprintType, Const)
class UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UEquipmentInstance* EquipItem(TSubclassOf<UEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UEquipmentInstance* ItemInstance);

	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;

	// Returns the first equipped instance of a given type, or nullptr if none are found
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UEquipmentInstance> InstanceType);

	// Returns all equipped instances of a given type, or an empty array if none are found
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FEquipmentList EquipmentList;
};
