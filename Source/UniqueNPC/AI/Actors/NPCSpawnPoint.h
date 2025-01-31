// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCSpawnPoint.generated.h"

class UBillboardComponent;

UCLASS()
class UNIQUENPC_API ANPCSpawnPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANPCSpawnPoint();

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite, Category = "Spawn Point")
	UBillboardComponent* SpawnPointIcon;

protected:
	virtual void BeginPlay() override;
};
