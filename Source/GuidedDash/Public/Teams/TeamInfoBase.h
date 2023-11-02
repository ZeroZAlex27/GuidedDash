// Copyright notice

#pragma once

#include "GameFramework/Info.h"
#include "System/GameplayTagStack.h"

#include "TeamInfoBase.generated.h"

namespace EEndPlayReason { enum Type : int; }

class UTeamCreationComponent;
class UTeamSubsystem;

UCLASS(Abstract)
class GUIDEDDASH_API ATeamInfoBase : public AInfo
{
	GENERATED_BODY()
	
public:
	ATeamInfoBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	int32 GetTeamId() const { return TeamId; }

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void RegisterWithTeamSubsystem(UTeamSubsystem* Subsystem);
	void TryRegisterWithTeamSubsystem();

private:
	void SetTeamId(int32 NewTeamId);

	UFUNCTION()
	void OnRep_TeamId();

public:
	friend UTeamCreationComponent;

	UPROPERTY(Replicated)
	FGameplayTagStackContainer TeamTags;

private:
	UPROPERTY(ReplicatedUsing = OnRep_TeamId)
	int32 TeamId;
};
