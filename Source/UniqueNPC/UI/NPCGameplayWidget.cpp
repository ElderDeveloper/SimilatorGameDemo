// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCGameplayWidget.h"

#include "Hotbar/NPCHotbarItemHolder.h"
#include "Hotbar/DragDrop/NPCItemDragDropOperation.h"
#include "UniqueNPC/Gameplay/NPCPlayerCharacter.h"
#include "UniqueNPC/Components/NPCInteractionComponent.h"
#include "UniqueNPC/Libraries/NPCGameplayLibrary.h"
#include "UniqueNPC/Subsystem/NPCPlayerInventorySubsystem.h"

void UNPCGameplayWidget::OnInteractableActorStateChanged_Implementation(AActor* InteractableActor, bool bIsInteractable)
{
}

void UNPCGameplayWidget::BindInteractionComponent()
{
	// Give Time for the owning player pawn to find the interaction component
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]
	{
		if (GetOwningPlayerPawn())
		{
			if (const auto NPCPlayer = Cast<ANPCPlayerCharacter>(GetOwningPlayerPawn()))
			{
				if (NPCPlayer->GetInteractionComponent())
				{
					NPCPlayer->GetInteractionComponent()->OnFoundInteractNPC.AddDynamic(this, &UNPCGameplayWidget::OnFoundInteractNPC);
					NPCPlayer->GetInteractionComponent()->OnLostInteractNPC.AddDynamic(this, &UNPCGameplayWidget::OnLostInteractNPC);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("UNPCGameplayWidget::NativeConstruct Owning player pawn has no interaction component"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("UNPCGameplayWidget::NativeConstruct Owning player pawn is not of type ANPCPlayerCharacter"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UNPCGameplayWidget::NativeConstruct No owning player pawn"));
		}
	},0.1f, false);
}


void UNPCGameplayWidget::OnFoundInteractNPC(AActor* InteractableNPC)
{
	OnInteractableActorStateChanged(InteractableNPC, true);
}


void UNPCGameplayWidget::OnLostInteractNPC(AActor* InteractableNPC)
{
	OnInteractableActorStateChanged(InteractableNPC, false);
}


void UNPCGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
	BindInteractionComponent();
}



