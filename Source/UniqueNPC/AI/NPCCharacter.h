// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actions/NPCActionBase.h"
#include "GameFramework/Character.h"
#include "UniqueNPC/Interface/NPCInteractionInterface.h"
#include "UniqueNPC/Interface/NPCWaitLineInterface.h"
#include "NPCCharacter.generated.h"

class UNPCActionManagerComponent;
class UBehaviorTree;


UENUM(Blueprintable, BlueprintType)
enum class ENPCCharacter : uint8
{
	Joyful,
	Angry,
	Scared
};
	
	

UCLASS()
class UNIQUENPC_API ANPCCharacter : public ACharacter , public INPCInteractionInterface , public INPCWaitLineInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNPCActionManagerComponent* ActionManagerComponent;

	UFUNCTION(BlueprintPure, Category = "AI")
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	
	UFUNCTION(BlueprintCallable, Category = "AI")
	void PlayerDialogueChoice(FGameplayTag ResponseTag);

	UPROPERTY(EditAnywhere, Category = "AI")
	float NPCRoamDuration = 120.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "AI")
	ENPCCharacter NPCCharacterType = ENPCCharacter::Joyful;

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Montages")
	UBehaviorTree* BehaviorTree;

 	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Montages")
	TArray<UAnimMontage*> GreetMontage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Montages")
	TArray<UAnimMontage*> InteractMontage;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Montages")
	TArray<UAnimMontage*> InsultMontage;

	UFUNCTION(BlueprintCallable)
	void MoveToNextLine(FVector TargetLocation, uint8 LineIndex);
	
	UFUNCTION(BlueprintCallable)
	void PlayRandomMontage(const TArray<UAnimMontage*>& Montages);

	bool bCurrentlyInteracting = false;
	float CurrentRoamDuration = 0.0f;
	bool bIsLeaving = false;

	UPROPERTY()
	ANPCWaitLine* CurrentWaitLine = nullptr;
	
	void UpdateBlackboardBool(const FName KeyName, const bool Value);

	virtual bool OnInteract_Implementation(ACharacter* InteractingCharacter) override;
	virtual bool OnStopInteract_Implementation(ACharacter* InteractingCharacter) override;
	virtual bool CanInteract_Implementation(ACharacter* InteractingCharacter) override;
	virtual bool CanStopInteract_Implementation(ACharacter* InteractingCharacter) override;
	virtual bool IsNPCCharacter_Implementation() override { return true; }
	
	virtual void MoveToLine_Implementation(ANPCWaitLine* Line, const FVector TargetLocation, const uint8 LineIndex) override;
	virtual void RequestWaitLine_Implementation(ANPCWaitLine* Line) override;
	virtual bool IsShopping_Implementation() override;

	UFUNCTION()
	void OnActionFinished(UNPCActionBase* FinishedAction);
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
