// Copyright notice

#pragma once

#include "GameFramework/HUD.h"
#include "Templates/SubclassOf.h"

#include "GuidedHUD.generated.h"

class UUserWidget;
class APlayerController;

/*
 *	AGuidedHUD
 * 
 *	Main HUD class used by this project.
 */
UCLASS()
class GUIDEDDASH_API AGuidedHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AGuidedHUD();

	void CreateHUD();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Initialization", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> MainWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> MainWidget;
};
