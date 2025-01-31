// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCItemSettings.h"
#include "NPCData.h"


FInventoryItem UNPCItemSettings::GetItemDetails(FGameplayTag ItemTag) const
{
	if (NPCItemTable)
	{
		UDataTable* Table = NPCItemTable.LoadSynchronous();
		if (FInventoryItem* FoundRow = Table->FindRow<FInventoryItem>(ItemTag.GetTagName(), TEXT("")))
		{
			return *FoundRow;
		}

		TArray<FInventoryItem*> Rows;
		Table->GetAllRows<FInventoryItem>(TEXT(""), Rows);
		for (FInventoryItem* Row : Rows)
		{
			if (Row->ItemTag.MatchesTag(ItemTag))
			{
				return *Row;
			}
		}
	}
	return FInventoryItem();
}
