// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UniqueNPC/Data/NPCData.h"
#include "NPCPlayerInventorySubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemChanged,const FInventoryItem&, Item, uint8, ItemSlotIndex );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventorySlotCleared, uint8, ItemSlotIndex );


UCLASS()
class UNIQUENPC_API UNPCPlayerInventorySubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable , Category = "Inventory")
	bool AddItem(FGameplayTag ItemTag, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable , Category = "Inventory")
	bool MoveItemToSlot(FGameplayTag ItemTag, uint8 SlotIndex);

	UFUNCTION(BlueprintCallable , Category = "Inventory")
	bool SwitchItemSlots(uint8 SlotIndexA, uint8 SlotIndexB);

	UFUNCTION(BlueprintCallable , Category = "Inventory")
	bool RemoveItem(FGameplayTag ItemTag, int32 Quantity = 1);
	
	UFUNCTION(BlueprintCallable , Category = "Inventory")
	void InitializeInventory(TSoftObjectPtr<UDataTable> InventoryTable , int32 InitialInventorySize);
	
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryItemChanged OnInventoryItemChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventorySlotCleared OnInventorySlotCleared;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	TArray<FInventoryItem> InventoryItems;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	TSoftObjectPtr<UDataTable> InventoryItemsTable;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	uint8 InventorySize = 10;

	FInventoryItem* FindInventoryItemDetails(FGameplayTag ItemTag) const;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController);

public:

	UFUNCTION(BlueprintCallable, Category = "NPC")
	void DropInventoryItem(uint8 SlotIndex, const FVector& Location, const FRotator& Rotation , int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "NPC")
	void GiveRandomItemToPlayer();

	UFUNCTION(BlueprintPure, Category = "NPC")
	FInventoryItem GetRandomItem();

	UFUNCTION(BlueprintCallable, Category = "NPC")
	void SpawnRandomItem(const FVector& Location, const FRotator& Rotation);

	UFUNCTION(BlueprintPure , Category = "Inventory")
	uint8 GetInventorySize() const { return InventorySize; }

	UFUNCTION(BlueprintPure , Category = "Inventory")
	bool GetItemAtIndex(uint8 SlotIndex, FInventoryItem& Item) const;

	UFUNCTION(BlueprintPure , Category = "Inventory")
	FInventoryItem GetItemWithTag(FGameplayTag ItemTag, uint8& FoundSlotIndex) const;
	
	UFUNCTION(BlueprintPure , Category = "Inventory")
	int32 GetItemQuantity(FGameplayTag ItemTag) const;

	UFUNCTION(BlueprintPure , Category = "Inventory")
	bool HasItem(FGameplayTag ItemTag,uint8& FoundSlotIndex) const;

	UFUNCTION(BlueprintPure , Category = "Inventory")
	bool HasEmptySlot(uint8& SlotIndex) const;
};
