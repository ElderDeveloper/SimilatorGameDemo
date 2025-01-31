// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDialogueEntry.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UNPCDialogueEntry::DialogueButtonClicked()
{
	OnDialogueButtonClicked.Broadcast(DialogueTag);
}


void UNPCDialogueEntry::SetupDialogueEntry(const FGameplayTag& Tag, const FText& Text)
{
	DialogueTag = Tag;
	DialogueText->SetText(Text);
}


void UNPCDialogueEntry::NativeConstruct()
{
	Super::NativeConstruct();

	if (DialogueButton)
	{
		DialogueButton->OnClicked.AddDynamic(this, &UNPCDialogueEntry::DialogueButtonClicked);
	}
}
