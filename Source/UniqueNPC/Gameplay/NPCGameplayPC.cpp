// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCGameplayPC.h"
#include "EnhancedInputSubsystems.h"
#include "UniqueNPC/UI/NPCGameplayWidget.h"

UNPCHotbarWidget* ANPCGameplayPC::GetHotbarWidget() const
{
	if (GameplayWidget)
	{
		return GameplayWidget->HotbarWidget;
	}
	return nullptr;
}

void ANPCGameplayPC::BeginPlay()
{
	Super::BeginPlay();
	
	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	if (IsValid(GameplayWidgetClass))
	{
		GameplayWidget = CreateWidget<UNPCGameplayWidget>(this, GameplayWidgetClass);
		GameplayWidget->AddToViewport();
	}
	
}
