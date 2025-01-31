// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "NPCDialogueEntry.generated.h"

struct FGameplayTag;
class UTextBlock;
class UButton;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueButtonClicked, FGameplayTag, DialogueResponse);

UCLASS()
class UNIQUENPC_API UNPCDialogueEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UButton* DialogueButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* DialogueText;

	UPROPERTY(BlueprintAssignable)
	FOnDialogueButtonClicked OnDialogueButtonClicked;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag DialogueTag;

	UFUNCTION(BlueprintCallable)
	void SetupDialogueEntry(const FGameplayTag& Tag, const FText& Text);

protected:
	
	UFUNCTION()
	void DialogueButtonClicked();
	
	virtual void NativeConstruct() override;
};
