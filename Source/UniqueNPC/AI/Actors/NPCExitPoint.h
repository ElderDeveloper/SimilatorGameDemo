// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCExitPoint.generated.h"

class UBoxComponent;
class UBillboardComponent;

UCLASS()
class UNIQUENPC_API ANPCExitPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANPCExitPoint();

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, Category = "Spawn Point")
	UBillboardComponent* ExitPointIcon;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, Category = "Spawn Point")
	UBoxComponent* ExitPointTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Point")
	FVector BoxExtent = FVector(100.0f);

protected:
	UFUNCTION()
	void OnExitPointTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult);
	
	virtual void BeginPlay() override;

};
