// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCGameplayWidget.generated.h"

class UNPCHotbarWidget;
/**
 * 
 */
UCLASS()
class UNIQUENPC_API UNPCGameplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), EditAnywhere)
	UNPCHotbarWidget* HotbarWidget;

	void BindInteractionComponent();
	
	UFUNCTION(BlueprintNativeEvent)
	void OnInteractableActorStateChanged(AActor* InteractableActor, bool bIsInteractable);
	
	UFUNCTION()
	void OnFoundInteractNPC(AActor* InteractableNPC);
	UFUNCTION()
	void OnLostInteractNPC(AActor* InteractableNPC);
	
	virtual void NativeConstruct() override;

};
