// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UniqueNPC/Subsystem/NPCPlayerInventorySubsystem.h"
#include "NPCHotbarWidget.generated.h"

class UNPCHotbarItemHolder;
class UHorizontalBox;

UCLASS()
class UNIQUENPC_API UNPCHotbarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNPCHotbarItemHolder> ItemHolderWidgetClass;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* ItemContainer;

	UPROPERTY(BlueprintReadOnly)
	TArray<UNPCHotbarItemHolder*> ItemHolders;

protected:

	UFUNCTION()
	void OnInventoryItemChanged(const FInventoryItem&  Item, uint8 ItemSlotIndex);
	UFUNCTION()
	void OnInventorySlotCleared(uint8 ItemSlotIndex);
	
	virtual void NativeConstruct() override;
};
