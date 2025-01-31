// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCActionBase.h"

#include "GameFramework/Character.h"
#include "UniqueNPC/Components/ActionManager/NPCActionManagerComponent.h"
#include "UniqueNPC/Libraries/NPCGameplayLibrary.h"


void UNPCActionBase::FinishAction()
{
	if (ActionManagerComponent.IsValid())
	{
		ActionManagerComponent->FinishAction(this);
	}
}

void UNPCActionBase::MontageEnded_Implementation(UAnimMontage* AnimMontage, bool bArg)
{
}

void UNPCActionBase::MontageBlendOut_Implementation(UAnimMontage* AnimMontage, bool bArg)
{
}

void UNPCActionBase::PlayMontage(UAnimMontage* Montage)
{
	if (Owner.IsValid() && Montage)
	{
		if (const auto Character = Cast<ACharacter>(Owner.Get()))
		{
			if (Character->GetMesh() && Character->GetMesh()->GetAnimInstance() )
			{
				Character->GetMesh()->GetAnimInstance()->Montage_Play(Montage);
				Character->GetMesh()->GetAnimInstance()->Montage_GetEndedDelegate()->BindUObject(this, &UNPCActionBase::MontageEnded);
				Character->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate()->BindUObject(this, &UNPCActionBase::MontageBlendOut);
			}
		}
	}


	//FinishAction();
}

void UNPCActionBase::Destroyed()
{
	OnDestroyed();

	/*
	if (Owner.IsValid())
	{
		if (const auto Character = Cast<ACharacter>(Owner.Get()))
		{
			if (Character->GetMesh() && Character->GetMesh()->GetAnimInstance() )
			{
				if (Character->GetMesh()->GetAnimInstance()->Montage_GetEndedDelegate()->IsBoundToObject(this))
				{
					Character->GetMesh()->GetAnimInstance()->Montage_GetEndedDelegate()->Unbind();
				}
				if (Character->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate()->IsBoundToObject(this))
				{
					Character->GetMesh()->GetAnimInstance()->Montage_GetBlendingOutDelegate()->Unbind();
				}
			}
		}
	}
	*/
}

void UNPCActionBase::Execute(AActor* OwnerActor)
{
	Owner = OwnerActor;
	OnExecute(OwnerActor);
}

void UNPCActionBase::Initialize(UNPCActionManagerComponent* ActionManager)
{
	ActionManagerComponent = ActionManager;
}


UNPCDirectorSubsystem* UNPCActionBase::GetNPCDirectorSubsystem() const
{
	if (Owner.IsValid())
	{
		return UNPCGameplayLibrary::GetNPCDirectorSubsystem(Owner->GetWorld());
	}
	return nullptr;
}


UNPCPlayerInventorySubsystem* UNPCActionBase::GetNPCPlayerInventorySubsystem() const
{
	if (Owner.IsValid())
	{
		return UNPCGameplayLibrary::GetNPCPlayerInventorySubsystem(Owner->GetWorld());
	}
	return nullptr;
}
