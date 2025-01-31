// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "UniqueNPC/Components/NPCInteractionComponent.h"
#include "NPCDialogueWidget.generated.h"

class USizeBox;
class UNPCDialogueEntry;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class UNIQUENPC_API UNPCDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly, meta = (BindWidget) )
	USizeBox* MainSizeBox;

	UPROPERTY(BlueprintReadOnly,VisibleDefaultsOnly, meta = (BindWidget) )
	UVerticalBox* DialogueContainer;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSubclassOf<UNPCDialogueEntry> DialogueEntryWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag,FText> DefaultDialogueOptions;

protected:

	UPROPERTY()
	AActor* LastInteractedNPC;
	
	UFUNCTION()
	void BindInteractionComponent(UNPCInteractionComponent* InteractionComponent);

	UFUNCTION()
	void OnNPCInteracted(AActor* InteractedNPC);

	UFUNCTION()
	void OnDialogueButtonClicked(FGameplayTag DialogueTag);
	
	virtual void NativeConstruct() override;
};
