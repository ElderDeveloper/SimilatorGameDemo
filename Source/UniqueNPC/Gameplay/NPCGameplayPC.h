// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NPCGameplayPC.generated.h"

class UNPCHotbarWidget;
class UNPCGameplayWidget;
class UInputMappingContext;


UCLASS()
class UNIQUENPC_API ANPCGameplayPC : public APlayerController
{
	GENERATED_BODY()

public:
	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UNPCGameplayWidget> GameplayWidgetClass;

	UFUNCTION(BlueprintPure, Category = "UI")
	UNPCGameplayWidget* GetGameplayWidget() const { return GameplayWidget; }

	UFUNCTION(BlueprintPure, Category = "UI")
	UNPCHotbarWidget* GetHotbarWidget() const;

protected:

	UPROPERTY()
	UNPCGameplayWidget* GameplayWidget;

	virtual void BeginPlay() override;
};
