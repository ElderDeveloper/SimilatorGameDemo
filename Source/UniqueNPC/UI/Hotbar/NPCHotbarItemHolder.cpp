// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCHotbarItemHolder.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DragDrop/NPCItemDragDropOperation.h"
#include "Kismet/KismetInputLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UniqueNPC/Libraries/NPCGameplayLibrary.h"
#include "UniqueNPC/Subsystem/NPCPlayerInventorySubsystem.h"
#include "DragDrop/NPCItemDragHolder.h"

void UNPCHotbarItemHolder::UpdateHotbarItem(const FInventoryItem& ItemUpdate)
{
	ItemData = ItemUpdate;
	if (ItemIcon && ItemQuantity)
	{
		if (ItemData.Icon)
		{
			ItemIcon->SetBrushFromTexture(ItemData.Icon);
			ItemIcon->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemData.Icon = ItemNoIcon;
			ItemIcon->SetBrushFromTexture(ItemNoIcon);
			ItemIcon->SetVisibility(ESlateVisibility::Visible);
		}

		if (ItemData.Quantity > 1)
		{
			ItemQuantity->SetText(FText::AsNumber(ItemData.Quantity));
			ItemQuantity->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
		}

		if(ItemData.Quantity == 0)
		{
			ClearHotbarItem();
		}
	}
}

void UNPCHotbarItemHolder::SetItemSlotIndex(uint8 SlotIndex)
{
	ItemSlotIndex = SlotIndex;
	if (ItemIndex)
	{
		FString IndexString = FString::Printf(TEXT("%d"), SlotIndex + 1);
		ItemIndex->SetText(FText::FromString(IndexString));
	}
}

void UNPCHotbarItemHolder::ClearHotbarItem()
{
	ItemData = FInventoryItem();
	if (ItemIcon && ItemQuantity)
	{
		ItemIcon->SetVisibility(ESlateVisibility::Hidden);
		ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UNPCHotbarItemHolder::HighlightItem(bool bHighlight)
{
	if (ItemHighlight->IsVisible())
	{
		if (const auto Inventory = UNPCGameplayLibrary::GetNPCPlayerInventorySubsystem(GetWorld()))
		{
			if (APawn* PlayerPawn = GetOwningPlayerPawn())
			{
				const FVector SpawnLocation = PlayerPawn->GetActorLocation() + PlayerPawn->GetActorForwardVector() * 150;
				Inventory->DropInventoryItem(ItemSlotIndex, SpawnLocation, FRotator::ZeroRotator, 1);
			}
		}
	}
	
	ItemHighlight->SetVisibility(ItemHighlight->IsVisible() ?  ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

}

void UNPCHotbarItemHolder::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UNPCHotbarItemHolder::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent)
{
	if (UKismetInputLibrary::PointerEvent_IsMouseButtonDown( InMouseEvent, EKeys::LeftMouseButton))
	{
		FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent , this, EKeys::LeftMouseButton);
		return EventReply.NativeReply;
	}
	
 	return FReply::Unhandled();

}

void UNPCHotbarItemHolder::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (ItemData.ItemTag == FGameplayTag())
	{
		return;
	}

	if (const auto DragWidget = CreateWidget<UNPCItemDragHolder>(GetWorld(), DragHolderClass))
	{
		DragWidget->InitializeWidget(ItemData.Icon);
		OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UNPCItemDragDropOperation::StaticClass());
		OutOperation->DefaultDragVisual = DragWidget;
		OutOperation->Pivot = EDragPivot::MouseDown;
		if (const auto ItemDragOperation = Cast<UNPCItemDragDropOperation>(OutOperation))
		{
			ItemDragOperation->ItemData = ItemData;
			ItemDragOperation->SourceSlot = this;
		}
	}
}


bool UNPCHotbarItemHolder::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	if (const auto ItemDragOperation = Cast<UNPCItemDragDropOperation>(InOperation))
	{
		if (ItemDragOperation->SourceSlot != this)
		{
			if (const auto InventorySubsystem = UNPCGameplayLibrary::GetNPCPlayerInventorySubsystem(GetWorld()))
			{
				if (ItemData.ItemTag == FGameplayTag())
				{
					InventorySubsystem->MoveItemToSlot(ItemDragOperation->ItemData.ItemTag, ItemSlotIndex);
				}
				else
				{
					InventorySubsystem->SwitchItemSlots(ItemSlotIndex, ItemDragOperation->SourceSlot->ItemSlotIndex);
				}
			}

		}
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
