// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCGameplayLibrary.h"

#include "Kismet/KismetMathLibrary.h"
#include "UniqueNPC/Components/Inventory/NPCItemBase.h"
#include "UniqueNPC/Data/NPCItemSettings.h"
#include "UniqueNPC/Subsystem/NPCDirectorSubsystem.h"
#include "UniqueNPC/Subsystem/NPCPlayerInventorySubsystem.h"

UNPCDirectorSubsystem* UNPCGameplayLibrary::GetNPCDirectorSubsystem(const UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (UNPCDirectorSubsystem* DirectorSubsystem = WorldContextObject->GetWorld()->GetSubsystem<UNPCDirectorSubsystem>())
		{
			return DirectorSubsystem;
		}
	}
	return  nullptr;
}


UNPCPlayerInventorySubsystem* UNPCGameplayLibrary::GetNPCPlayerInventorySubsystem(const UObject* WorldContextObject)
{
	if (WorldContextObject)
	{
		if (WorldContextObject->GetWorld()->GetFirstLocalPlayerFromController())
		{
			if (UNPCPlayerInventorySubsystem* InventorySubsystem = WorldContextObject->GetWorld()->GetFirstLocalPlayerFromController()->GetSubsystem<UNPCPlayerInventorySubsystem>())
			{
				return InventorySubsystem;
			}
		}
	}
	return nullptr;
}


FInventoryItem UNPCGameplayLibrary::GetItemDetails(FGameplayTag ItemTag)
{
	if (const auto ItemSettings = GetMutableDefault<UNPCItemSettings>())
	{
		return ItemSettings->GetItemDetails(ItemTag);
	}
	return  FInventoryItem();
}


void UNPCGameplayLibrary::GiveRandomItemToPlayer(const UObject* WorldContextObject)
{
	if (const auto PlayerInventorySubsystem = UNPCGameplayLibrary::GetNPCPlayerInventorySubsystem(WorldContextObject))
	{
		PlayerInventorySubsystem->GiveRandomItemToPlayer();
	}
}


FInventoryItem UNPCGameplayLibrary::GetRandomItem(const UObject* WorldContextObject)
{
	if (const auto PlayerInventorySubsystem = UNPCGameplayLibrary::GetNPCPlayerInventorySubsystem(WorldContextObject))
	{
		return PlayerInventorySubsystem->GetRandomItem();
	}
	return FInventoryItem();
}


void UNPCGameplayLibrary::SpawnRandomItem(const UObject* WorldContextObject, const FVector& Location, const FRotator& Rotation)
{
	if (const auto PlayerInventorySubsystem = UNPCGameplayLibrary::GetNPCPlayerInventorySubsystem(WorldContextObject))
	{
		 PlayerInventorySubsystem->SpawnRandomItem(Location,Rotation);
	}
}
