// Copyright notice

#pragma once

#include "Engine/DataAsset.h"

#include "PawnData.generated.h"

class UAbilitySet;
class UAbilityTagRelationshipMapping;
class UGuidedInputConfig;

/*
 *	UPawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class GUIDEDDASH_API UPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPawnData(const FObjectInitializer& ObjectInitializer);

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guided|Abilities")
	TArray<TObjectPtr<UAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guided|Abilities")
	TObjectPtr<UAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Guided|Input")
	TObjectPtr<UGuidedInputConfig> InputConfig;
};
