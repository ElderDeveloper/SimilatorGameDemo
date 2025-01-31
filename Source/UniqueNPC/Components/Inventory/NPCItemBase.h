// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UniqueNPC/Data/NPCData.h"
#include "UniqueNPC/Interface/NPCInteractionInterface.h"
#include "NPCItemBase.generated.h"

class USphereComponent;

UCLASS()
class UNIQUENPC_API ANPCItemBase : public AActor , public INPCInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANPCItemBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	USphereComponent* SphereComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	bool bShouldCollectOnOverlap = true;

	UFUNCTION(BlueprintCallable)
	void DestroyItem();

	UFUNCTION(BlueprintCallable)
	void InitializeItem(const FInventoryItem& NewItemData);
	
	virtual bool OnInteract_Implementation(ACharacter* InteractingCharacter) override;
	virtual bool OnStopInteract_Implementation(ACharacter* InteractingCharacter) override { return true; }
	virtual bool CanInteract_Implementation(ACharacter* InteractingCharacter) override { return true; }
	virtual bool CanStopInteract_Implementation(ACharacter* InteractingCharacter) override { return true; }
	virtual bool IsNPCCharacter_Implementation() override { return false; }

protected:
	
	FInventoryItem ItemData;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;

};
