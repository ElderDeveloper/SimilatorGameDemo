// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCHotbarWidget.h"
#include "NPCHotbarItemHolder.h"
#include "Components/HorizontalBox.h"
#include "UniqueNPC/Libraries/NPCGameplayLibrary.h"


void UNPCHotbarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (const auto InventorySubsystem = UNPCGameplayLibrary::GetNPCPlayerInventorySubsystem(this))
	{
		uint8 Size = InventorySubsystem->GetInventorySize();

		if (ItemHolderWidgetClass && ItemContainer)
		{
			for (uint8 i = 0; i < Size; i++)
			{
				if (UNPCHotbarItemHolder* ItemHolderWidget = CreateWidget<UNPCHotbarItemHolder>(GetWorld(), ItemHolderWidgetClass))
				{
					ItemContainer->AddChild(ItemHolderWidget);
					ItemHolderWidget->UpdateHotbarItem(FInventoryItem(FGameplayTag(),0));
					ItemHolderWidget->SetItemSlotIndex(ItemHolders.Add(ItemHolderWidget));
				}
			}
		}
		
		InventorySubsystem->OnInventoryItemChanged.AddDynamic(this, &UNPCHotbarWidget::OnInventoryItemChanged);
		InventorySubsystem->OnInventorySlotCleared.AddDynamic(this, &UNPCHotbarWidget::OnInventorySlotCleared);
	}
	
}

void UNPCHotbarWidget::OnInventoryItemChanged(const FInventoryItem& Item, uint8 ItemSlotIndex)
{
	if (ItemHolders.IsValidIndex(ItemSlotIndex))
	{
		// if == Item.Tag
		if (ItemHolders[ItemSlotIndex])
		{
			ItemHolders[ItemSlotIndex]->UpdateHotbarItem(Item);
		}
	}
}

void UNPCHotbarWidget::OnInventorySlotCleared(uint8 ItemSlotIndex)
{
	if (ItemHolders.IsValidIndex(ItemSlotIndex))
	{
		if (ItemHolders[ItemSlotIndex])
		{
			ItemHolders[ItemSlotIndex]->ClearHotbarItem();
		}
	}
}
