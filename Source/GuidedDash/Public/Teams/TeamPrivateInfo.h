// Copyright notice

#pragma once

#include "Teams/TeamInfoBase.h"

#include "TeamPrivateInfo.generated.h"

/*
 *	ATeamPrivateInfo
 *
 *	Used to store private info of a team
 */
UCLASS()
class GUIDEDDASH_API ATeamPrivateInfo : public ATeamInfoBase
{
	GENERATED_BODY()
	
public:
	ATeamPrivateInfo(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
};
