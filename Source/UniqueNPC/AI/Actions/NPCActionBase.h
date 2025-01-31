// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "NPCActionBase.generated.h"

class UNPCActionManagerComponent;
class UNPCPlayerInventorySubsystem;
class UNPCDirectorSubsystem;
/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class UNIQUENPC_API UNPCActionBase : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnExecute(AActor* OwnerActor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroyed();

	UFUNCTION(BlueprintCallable)
	void FinishAction();

	UFUNCTION(BlueprintNativeEvent)
	void MontageEnded(UAnimMontage* AnimMontage, bool bArg);

	UFUNCTION(BlueprintNativeEvent)
	void MontageBlendOut(UAnimMontage* AnimMontage, bool bArg);
	
	UFUNCTION(BlueprintCallable)
	void PlayMontage(UAnimMontage* Montage);
	
	virtual void Destroyed();
	virtual void Execute(AActor* OwnerActor);
	virtual void Initialize(UNPCActionManagerComponent* ActionManager);

protected:
	UPROPERTY()
	TSoftObjectPtr<AActor> Owner;
	UPROPERTY()
	TSoftObjectPtr<UNPCActionManagerComponent> ActionManagerComponent;

public:

	UFUNCTION(BlueprintPure, Category = "Action")
	AActor* GetOwner() const { return Owner.Get(); }

	UFUNCTION(BlueprintPure, Category = "Action")
	UNPCDirectorSubsystem* GetNPCDirectorSubsystem() const;

	UFUNCTION(BlueprintPure, Category = "Action")
	UNPCPlayerInventorySubsystem* GetNPCPlayerInventorySubsystem() const;
};
