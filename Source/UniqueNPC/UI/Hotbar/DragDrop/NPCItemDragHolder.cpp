// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCItemDragHolder.h"

#include "Components/Image.h"

void UNPCItemDragHolder::InitializeWidget(UTexture2D* ItemTexture2D)
{
	if (ItemIcon)
	{
		ItemIcon->SetBrushFromTexture(ItemTexture2D);
	}
}
