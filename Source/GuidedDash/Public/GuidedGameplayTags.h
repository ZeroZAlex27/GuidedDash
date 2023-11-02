// Copyright notice

#pragma once

#include "NativeGameplayTags.h"

namespace GuidedGameplayTags
{
	GUIDEDDASH_API	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	// Declare all of the custom native tags that GuidedDash will use
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_IsDead);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cooldown);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Cost);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsBlocked);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_TagsMissing);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_Networking);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_ActivateFail_ActivationGroup);

	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Behavior_SurvivesDeath);

	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Mouse);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look_Stick);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Crouch);

	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Death);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Reset);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_RequestReset);

	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Crouching);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dying);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Death_Dead);

	// These are mappings from MovementMode enums to GameplayTags associated with those enums (below)
	GUIDEDDASH_API	extern const TMap<uint8, FGameplayTag> MovementModeTagMap;
	GUIDEDDASH_API	extern const TMap<uint8, FGameplayTag> CustomMovementModeTagMap;

	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Walking);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_NavWalking);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Falling);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Swimming);
	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Flying);

	GUIDEDDASH_API	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Movement_Mode_Custom);
};
