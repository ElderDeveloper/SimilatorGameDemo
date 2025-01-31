// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UniqueNPC/Data/NPCData.h"
#include "NPCItemDragHolder.generated.h"

class UImage;


UCLASS()
class UNIQUENPC_API UNPCItemDragHolder : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Item")
	UImage* ItemIcon;

	void InitializeWidget(UTexture2D* ItemTexture2D);
	
};

