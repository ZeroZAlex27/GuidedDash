// Copyright notice

#include "AbilitySystem/Attributes/GuidedAttributeSet.h"
// Local
#include "AbilitySystem/GuidedAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GuidedAttributeSet)

class UWorld;

UWorld* UGuidedAttributeSet::GetWorld() const
{
	const UObject* Outer = GetOuter();
	check(Outer);

	return Outer->GetWorld();
}

UGuidedAbilitySystemComponent* UGuidedAttributeSet::GetGuidedAbilitySystemComponent() const
{
	return Cast<UGuidedAbilitySystemComponent>(GetOwningAbilitySystemComponent());
}