// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCPlayerInventorySubsystem.h"

#include "Kismet/KismetMathLibrary.h"
#include "UniqueNPC/Components/Inventory/NPCItemBase.h"
#include "UniqueNPC/Data/NPCItemSettings.h"


bool UNPCPlayerInventorySubsystem::AddItem(FGameplayTag ItemTag, int32 Quantity)
{
	uint8 SlotIndex;
	if (HasItem(ItemTag, SlotIndex))
	{
		InventoryItems[SlotIndex].Quantity += Quantity;
		OnInventoryItemChanged.Broadcast(InventoryItems[SlotIndex], SlotIndex);
		UE_LOG(LogTemp, Log, TEXT("Updated %d %s in inventory"), Quantity, *ItemTag.ToString());
		return true;
	}
	if (HasEmptySlot(SlotIndex))
	{
		InventoryItems[SlotIndex].ItemTag = ItemTag;
		InventoryItems[SlotIndex].Quantity = Quantity;
		UE_LOG(LogTemp, Log, TEXT("Added %d %s to inventory"), Quantity, *ItemTag.ToString());
		if (FInventoryItem* Details = FindInventoryItemDetails(ItemTag))
		{
			InventoryItems[SlotIndex].ItemName = Details->ItemName;
			InventoryItems[SlotIndex].Description = Details->Description;
			InventoryItems[SlotIndex].Icon = Details->Icon;
			InventoryItems[SlotIndex].ItemActorClass = Details->ItemActorClass;
			InventoryItems[SlotIndex].bIsStackable = Details->bIsStackable;
		}
		OnInventoryItemChanged.Broadcast(InventoryItems[SlotIndex], SlotIndex);
		return true;
	}
	return false;
}

bool UNPCPlayerInventorySubsystem::MoveItemToSlot(FGameplayTag ItemTag, uint8 SlotIndex)
{
	uint8 FoundSlotIndex;
	FInventoryItem Item = GetItemWithTag(ItemTag,FoundSlotIndex);
	
	if (Item.ItemTag.IsValid())
	{
		if (InventoryItems.IsValidIndex(SlotIndex))
		{
			InventoryItems[SlotIndex] = Item;
			OnInventoryItemChanged.Broadcast(InventoryItems[SlotIndex], SlotIndex);

			InventoryItems[FoundSlotIndex].ItemTag = FGameplayTag();
			InventoryItems[FoundSlotIndex].Quantity = 0;
			OnInventorySlotCleared.Broadcast(FoundSlotIndex);
			return true;
		}
	}

	return false;
}

bool UNPCPlayerInventorySubsystem::SwitchItemSlots(uint8 SlotIndexA, uint8 SlotIndexB)
{
	if (InventoryItems.IsValidIndex(SlotIndexA) && InventoryItems.IsValidIndex(SlotIndexB))
	{
		FInventoryItem TempItem = InventoryItems[SlotIndexA];
		InventoryItems[SlotIndexA] = InventoryItems[SlotIndexB];
		InventoryItems[SlotIndexB] = TempItem;
		OnInventoryItemChanged.Broadcast(InventoryItems[SlotIndexA], SlotIndexA);
		OnInventoryItemChanged.Broadcast(InventoryItems[SlotIndexB], SlotIndexB);
		return true;
	}
	return false;
}


bool UNPCPlayerInventorySubsystem::RemoveItem(FGameplayTag ItemTag, int32 Quantity)
{
	uint8 SlotIndex;
	if (HasItem(ItemTag, SlotIndex))
	{
		InventoryItems[SlotIndex].Quantity -= Quantity;
		if (InventoryItems[SlotIndex].Quantity <= 0)
		{
			InventoryItems[SlotIndex].ItemTag = FGameplayTag();
			InventoryItems[SlotIndex].Quantity = 0;
			OnInventorySlotCleared.Broadcast(SlotIndex);
		}
		else
		{
			OnInventoryItemChanged.Broadcast(InventoryItems[SlotIndex], SlotIndex);
		}
		return true;
	}
	return false;
}


bool UNPCPlayerInventorySubsystem::HasItem(FGameplayTag ItemTag, uint8& FoundSlotIndex) const
{
	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{
		if (InventoryItems[i].ItemTag == ItemTag)
		{
			FoundSlotIndex = i;
			return true;
		}
	}
	FoundSlotIndex = INDEX_NONE;
	return false;
}

bool UNPCPlayerInventorySubsystem::GetItemAtIndex(uint8 SlotIndex, FInventoryItem& Item) const
{
	if (InventoryItems.IsValidIndex(SlotIndex))
	{
		if (InventoryItems[SlotIndex].ItemTag.IsValid())
		{
			Item = InventoryItems[SlotIndex];
			return true;
		}
	}
	return false;
}


bool UNPCPlayerInventorySubsystem::HasEmptySlot(uint8& SlotIndex) const
{
	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{
		if (InventoryItems[i].ItemTag.IsValid() == false)
		{
			SlotIndex = i;
			return true;
		}
	}
	SlotIndex = INDEX_NONE;
	return false;
}


int32 UNPCPlayerInventorySubsystem::GetItemQuantity(FGameplayTag ItemTag) const
{
	uint8 SlotIndex;
	if (HasItem(ItemTag, SlotIndex))
	{
		return InventoryItems[SlotIndex].Quantity;
	}
	return 0;
}


FInventoryItem UNPCPlayerInventorySubsystem::GetItemWithTag(FGameplayTag ItemTag, uint8& FoundSlotIndex) const
{
	FoundSlotIndex = INDEX_NONE;
	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{
		if (InventoryItems[i].ItemTag == ItemTag)
		{
			FoundSlotIndex = i;
			return InventoryItems[i];
		}
	}
	return FInventoryItem();
}


void UNPCPlayerInventorySubsystem::InitializeInventory(TSoftObjectPtr<UDataTable>InventoryTable, int32 InitialInventorySize)
{
	UDataTable* ItemsTable = InventoryTable.LoadSynchronous();
	if (ItemsTable == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Table is not set"));
	}
	InventorySize = InitialInventorySize;
	InventoryItemsTable = InventoryTable;
	
	InventoryItems.Empty(InitialInventorySize);
	InventoryItems.Init(FInventoryItem(), InitialInventorySize);
}


FInventoryItem* UNPCPlayerInventorySubsystem::FindInventoryItemDetails(FGameplayTag ItemTag) const
{
	if (UDataTable* ItemsTable = InventoryItemsTable.LoadSynchronous())
	{
		TArray<FInventoryItem*> Rows;
		ItemsTable->GetAllRows<FInventoryItem>(TEXT(""), Rows);
		for (const auto& Row : Rows)
		{
			if (Row->ItemTag.MatchesTag(ItemTag))
			{
				return Row;
			}
		}
	}

	return nullptr;
}


void UNPCPlayerInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}


void UNPCPlayerInventorySubsystem::PlayerControllerChanged(APlayerController* NewPlayerController)
{
	Super::PlayerControllerChanged(NewPlayerController);

	if (NewPlayerController)
	{
		const auto ItemSettings = GetMutableDefault<UNPCItemSettings>();
		InitializeInventory(ItemSettings->NPCItemTable, ItemSettings->PlayerInventorySize);
	}
}

void UNPCPlayerInventorySubsystem::DropInventoryItem(uint8 SlotIndex, const FVector& Location, const FRotator& Rotation,int32 Quantity)
{
	FInventoryItem Item;
	if (GetItemAtIndex(SlotIndex, Item))
	{
		if (Item.ItemActorClass.IsValid())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			if (const auto SpawnedItem = GetWorld()->SpawnActor<ANPCItemBase>(Item.ItemActorClass.Get(), Location, Rotation, SpawnParams))
			{
				if (!Item.ItemMesh)
				{
					if (const auto con = FindInventoryItemDetails(Item.ItemTag))
					{
						Item.ItemMesh = con->ItemMesh;
					}
				}
				SpawnedItem->InitializeItem(Item);
				RemoveItem(Item.ItemTag, Quantity);
			}
		}
	}
}


void UNPCPlayerInventorySubsystem::GiveRandomItemToPlayer()
{
	const auto Item = GetRandomItem();
	AddItem(Item.ItemTag, 1);
}


FInventoryItem UNPCPlayerInventorySubsystem::GetRandomItem()
{
	if (const auto ItemSettings = GetMutableDefault<UNPCItemSettings>())
	{
		if (UDataTable* NPCItemTable = ItemSettings->NPCItemTable.LoadSynchronous())
		{
			TArray<FName> RowNames = NPCItemTable->GetRowNames();
			if (RowNames.Num() == 0) return FInventoryItem();

			// Select a random row name
			int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, RowNames.Num() - 1);
			FName RandomRowName = RowNames[RandomIndex];

			if (const auto Item = NPCItemTable->FindRow<FInventoryItem>(RandomRowName, TEXT("Random Row Lookup")))
			{
				return *Item;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("UNPCPlayerInventorySubsystem: NPC Item Table is not valid!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UNPCPlayerInventorySubsystem: Item Settings is not valid!"));
	}
	return FInventoryItem();
}


void UNPCPlayerInventorySubsystem::SpawnRandomItem(const FVector& Location, const FRotator& Rotation)
{
	const auto Item = GetRandomItem();
	if (Item.ItemActorClass.IsValid())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		if (const auto SpawnedItem = GetWorld()->SpawnActor<ANPCItemBase>(Item.ItemActorClass.Get(), Location, Rotation,SpawnParams))
		{
			SpawnedItem->InitializeItem(Item);
		}
	}
}
