// Copyright notice

#include "AbilitySystem/Executions/DamageExecution.h"
// Engine
#include "Math/UnrealMathUtility.h"
// Local
#include "AbilitySystem/Attributes/HealthSet.h"
#include "AbilitySystem/Attributes/CombatSet.h"

struct FDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition BaseDamageDef;

	FDamageStatics()
	{
		BaseDamageDef = FGameplayEffectAttributeCaptureDefinition(UCombatSet::GetBaseDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
};

static FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UDamageExecution::UDamageExecution()
{
	// Any used attributes should be captured before "Execute_Implementation"
	RelevantAttributesToCapture.Add(DamageStatics().BaseDamageDef);
}

void UDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// Getting captured value from attribute
	float BaseDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BaseDamageDef, EvaluateParameters, BaseDamage);
	BaseDamage += FMath::Floor(FMath::RandRange(-0.25 * BaseDamage, 0.25 * BaseDamage)); // Applying 25% randomness to damage

	if (BaseDamage > 0.0f)
	{
		// Apply a damage modifier, this gets turned into -health on the target
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UHealthSet::GetDamageAttribute(), EGameplayModOp::Additive, BaseDamage));
	}
}