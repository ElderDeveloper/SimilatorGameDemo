// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"

#include "Actions/NPCActionBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/NPCAIController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UniqueNPC/Components/ActionManager/NPCActionManagerComponent.h"
#include "UniqueNPC/Data/NPCItemSettings.h"
#include "UniqueNPC/Subsystem/NPCDirectorSubsystem.h"
#include "UniqueNPC/AI/Actors/NPCWaitLine.h"



// Sets default values
ANPCCharacter::ANPCCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = ANPCAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	ActionManagerComponent = CreateDefaultSubobject<UNPCActionManagerComponent>(TEXT("ActionManagerComponent"));
}


void ANPCCharacter::PlayerDialogueChoice(FGameplayTag ResponseTag)
{

	if (ActionManagerComponent->ExecuteActionByTag(ResponseTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCCharacter: Executed Action: %s"), *ActionManagerComponent->GetCurrentAction()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NPCCharacter: Failed to execute action"));
	}
}


void ANPCCharacter::MoveToNextLine(FVector TargetLocation, uint8 LineIndex)
{

}


void ANPCCharacter::PlayRandomMontage(const TArray<UAnimMontage*>& Montages)
{
	if (Montages.Num() == 0) return;
	int32 RandomIndex = UKismetMathLibrary::RandomIntegerInRange(0, Montages.Num() - 1);
	if (UAnimMontage* RandomMontage = Montages[RandomIndex])
	{
		PlayAnimMontage(RandomMontage);
	}
}


void ANPCCharacter::UpdateBlackboardBool(const FName KeyName, const bool Value)
{
	if (const auto AIController = Cast<ANPCAIController>(GetController()))
	{
		AIController->GetBlackboardComponent()->SetValueAsBool(KeyName, Value);
	}
}


bool ANPCCharacter::OnInteract_Implementation(ACharacter* InteractingCharacter)
{
	if (InteractingCharacter)
	{
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), InteractingCharacter->GetActorLocation());
		SetActorRotation(FRotator(0.0f, TargetRotation.Yaw, 0.0f));
	}
	bCurrentlyInteracting = true;
	GetMovementComponent()->StopMovementImmediately();
	GetMovementComponent()->SetComponentTickEnabled(false);
	UpdateBlackboardBool(FName("IsInteracting") ,true);
	return true;
}


bool ANPCCharacter::OnStopInteract_Implementation(ACharacter* InteractingCharacter)
{
	bCurrentlyInteracting = false;
	GetMovementComponent()->SetComponentTickEnabled(true);
	UpdateBlackboardBool(FName("IsInteracting") ,false);
	return true;
}


bool ANPCCharacter::CanInteract_Implementation(ACharacter* InteractingCharacter)
{
	return bCurrentlyInteracting == false;
}


bool ANPCCharacter::CanStopInteract_Implementation(ACharacter* InteractingCharacter)
{
	return bCurrentlyInteracting;
}


void ANPCCharacter::MoveToLine_Implementation(ANPCWaitLine* Line, const FVector TargetLocation,const uint8 LineIndex)
{
	if (const auto AIController = Cast<ANPCAIController>(GetController()))
	{
		CurrentWaitLine = Line;
		AIController->MoveToLocation(TargetLocation, LineIndex);
		AIController->GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), TargetLocation);
		AIController->GetBlackboardComponent()->SetValueAsInt(FName("WaitLineIndex"), LineIndex);
		AIController->GetBlackboardComponent()->SetValueAsObject(FName("WaitLine"), Line);
	}
}

void ANPCCharacter::RequestWaitLine_Implementation(ANPCWaitLine* Line)
{
	if (!Line)
		return;
	
	if (const auto AIController = Cast<ANPCAIController>(GetController()))
	{
		CurrentWaitLine = Line;
		
		FVector TargetLocation;
		uint8 LineIndex;
		if (Line->RequestLineForActor( this, TargetLocation, LineIndex))
		{
			FString Location = TargetLocation.ToString();
			AIController->MoveToLocation(TargetLocation, LineIndex);
			AIController->GetBlackboardComponent()->SetValueAsVector(FName("TargetLocation"), TargetLocation);
			AIController->GetBlackboardComponent()->SetValueAsInt(FName("WaitLineIndex"), LineIndex);
			AIController->GetBlackboardComponent()->SetValueAsObject(FName("WaitLine"), Line);
			AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsShopping"), true);
		}
	}
}

bool ANPCCharacter::IsShopping_Implementation()
{
	if (bIsLeaving)
	{
		 return true;
	}
	return CurrentWaitLine != nullptr;
}

void ANPCCharacter::OnActionFinished(UNPCActionBase* FinishedAction)
{
	if (CurrentWaitLine)
	{
		CurrentWaitLine->LeaveLine(this);
		CurrentWaitLine = nullptr;

		if (const auto AIController = Cast<ANPCAIController>(GetController()))
		{
			AIController->GetBlackboardComponent()->SetValueAsBool(FName("NPCLeaving"), true);
			AIController->GetBlackboardComponent()->SetValueAsObject(FName("WaitLine"), nullptr);
			AIController->GetBlackboardComponent()->SetValueAsBool(FName("IsShopping"), false);
			bIsLeaving = true;
		}
	}
}


void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Randomize the character type
	NPCCharacterType = static_cast<ENPCCharacter>(UKismetMathLibrary::RandomIntegerInRange(0, 2));
	ActionManagerComponent->OnActionFinished.AddDynamic(this, &ANPCCharacter::OnActionFinished);
}


void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentRoamDuration += DeltaTime;
	if (CurrentRoamDuration >= NPCRoamDuration)
	{
		UpdateBlackboardBool(FName("NPCLeaving") ,true);
		bIsLeaving = true;
		CurrentRoamDuration = 0.0f;
	}
}

