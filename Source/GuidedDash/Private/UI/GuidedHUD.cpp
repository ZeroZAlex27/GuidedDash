// Copyright notice

#include "UI/GuidedHUD.h"
// Engine
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GuidedHUD)

AGuidedHUD::AGuidedHUD()
{
	MainWidget = nullptr;
	MainWidgetClass = nullptr;
}

void AGuidedHUD::CreateHUD()
{
	if (!MainWidget && MainWidgetClass)
	{
		MainWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MainWidgetClass);
		MainWidget->AddToViewport();
	}
}
