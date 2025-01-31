// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCDialogueWidget.h"
#include "NPCDialogueEntry.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "UniqueNPC/AI/NPCCharacter.h"
#include "UniqueNPC/Gameplay/NPCPlayerCharacter.h"
#include "UniqueNPC/Interface/NPCInteractionInterface.h"


void UNPCDialogueWidget::BindInteractionComponent(UNPCInteractionComponent* InteractionComponent)
{
	if (InteractionComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UNPCDialogueWidget::BindInteractionComponent Found Interaction Component"));
	}
}

void UNPCDialogueWidget::OnNPCInteracted(AActor* InteractedNPC)
{
	MainSizeBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	GetOwningPlayer()->SetShowMouseCursor(true);
	GetOwningPlayer()->SetInputMode(FInputModeUIOnly());
	LastInteractedNPC = InteractedNPC;


}

void UNPCDialogueWidget::OnDialogueButtonClicked(FGameplayTag DialogueTag)
{
	MainSizeBox->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->SetShowMouseCursor(false);
	GetOwningPlayer()->SetInputMode(FInputModeGameOnly());

	if (const auto PlayerCharacter = Cast<ANPCPlayerCharacter>(GetOwningPlayerPawn()))
	{
		PlayerCharacter->EnablePlayerMovement();
	}

	if (LastInteractedNPC)
	{
		if (INPCInteractionInterface* NPCInterface = Cast<INPCInteractionInterface>(LastInteractedNPC))
		{
			NPCInterface->OnStopInteract_Implementation(Cast<ACharacter>(GetOwningPlayerPawn()));
		}
		if (const auto NPCCharacter = Cast<ANPCCharacter>(LastInteractedNPC))
		{
			NPCCharacter->PlayerDialogueChoice(DialogueTag);
		}
	}
}

void UNPCDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (const auto PlayerCharacter = Cast<ANPCPlayerCharacter>(GetOwningPlayerPawn()))
	{
		PlayerCharacter->OnInteractionComponentChanged.AddDynamic(this, &UNPCDialogueWidget::BindInteractionComponent);
		PlayerCharacter->OnInteractedWithNPC.AddDynamic(this, &UNPCDialogueWidget::OnNPCInteracted);
	}


	if (DialogueEntryWidgetClass)
	{
		if (DialogueContainer)
		{
			DialogueContainer->ClearChildren();

			for (const auto& Pair : DefaultDialogueOptions)
			{
				if (auto DialogueEntry = CreateWidget<UNPCDialogueEntry>(GetWorld(), DialogueEntryWidgetClass))
				{
					DialogueEntry->SetupDialogueEntry(Pair.Key, Pair.Value);
					DialogueEntry->OnDialogueButtonClicked.AddDynamic(this, &UNPCDialogueWidget::OnDialogueButtonClicked);
					DialogueContainer->AddChild(DialogueEntry);
				}
			}
		}
	}
	
	if (MainSizeBox)
	{
		MainSizeBox->SetVisibility(ESlateVisibility::Collapsed);
	}

}
