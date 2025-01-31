// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UniqueNPC/Data/NPCData.h"
#include "NPCHotbarItemHolder.generated.h"

class UNPCItemDragHolder;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class UNIQUENPC_API UNPCHotbarItemHolder : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget), Category = "Hotbar")
	UImage* ItemIcon;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget), Category = "Hotbar")
	UTextBlock* ItemQuantity;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget), Category = "Hotbar")
	UTextBlock* ItemIndex;

	UPROPERTY(BlueprintReadOnly,meta=(BindWidget), Category = "Hotbar")
	UImage* ItemHighlight;

	UPROPERTY(EditDefaultsOnly, Category = "Hotbar")
	TSubclassOf<UNPCItemDragHolder> DragHolderClass;

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void UpdateHotbarItem(const FInventoryItem& ItemUpdate);

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void SetItemSlotIndex(uint8 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void ClearHotbarItem();

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	FInventoryItem GetItemData() const { return ItemData; }

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	uint8 GetItemSlotIndex() const { return ItemSlotIndex; }

	UFUNCTION(BlueprintCallable, Category = "Hotbar")
	void HighlightItem(bool bHighlight);

protected:
	
	FInventoryItem ItemData;

	UPROPERTY(BlueprintReadOnly, Category = "Hotbar")
	uint8 ItemSlotIndex;
	
	UPROPERTY(EditDefaultsOnly, Category = "Hotbar")
	UTexture2D* ItemNoIcon;

	virtual void NativeConstruct() override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
