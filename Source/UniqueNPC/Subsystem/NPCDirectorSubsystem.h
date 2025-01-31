// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/DeveloperSettings.h"
#include "NPCDirectorSubsystem.generated.h"


class ANPCWaitLine;
class ACharacter;

USTRUCT(BlueprintType)
struct FNPCSpawnRate
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MinDuration = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxDuration = 10.0f;
};

UCLASS(Config=Game, defaultconfig, meta = (DisplayName="NPC Settings"))
class UNPCSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config,BlueprintReadWrite, EditAnywhere, Category = "NPC Settings")
	uint8 TotalNPCs = 10;

	UPROPERTY(Config,BlueprintReadWrite, EditAnywhere, Category = "NPC Settings")
	FNPCSpawnRate SpawnRate;

	UPROPERTY(Config,BlueprintReadWrite, EditAnywhere, Category = "NPC Settings")
	bool bSpawnAllAtOnce = false;

	UPROPERTY(Config,BlueprintReadWrite, EditAnywhere, Category = "NPC Settings")
	TSubclassOf<ACharacter> NPCClass;

	UPROPERTY(Config,BlueprintReadWrite, EditAnywhere, Category = "NPC Settings")
	TSoftClassPtr<UDataTable> NPCVisualData;
};

UCLASS()
class UNIQUENPC_API UNPCDirectorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool AddSpawnPoint(AActor* SpawnPoint);

	UFUNCTION(BlueprintCallable)
	bool AddExitLocation(AActor* ExitLocation);

	UFUNCTION(BlueprintCallable)
	bool AddWaitLine(ANPCWaitLine* WaitLine);

	UFUNCTION(BlueprintCallable)
	void RequestNPCSpawn(bool bControlNPCLimits = true);

	UFUNCTION(BlueprintCallable)
	void RequestDeleteNPC(ACharacter* NPC);

	UFUNCTION(BlueprintPure)
	AActor* GetRandomSpawnPoint();

	UFUNCTION(BlueprintPure)
	AActor* GetRandomExitLocation();

protected:
	UPROPERTY()
	UNPCSettings* Settings;
	UPROPERTY()
	TArray<AActor*> SpawnPoints;
	UPROPERTY()
	TArray<AActor*> ExitLocations;
	UPROPERTY()
	TArray<ANPCWaitLine*> WaitLines;
	UPROPERTY()
	TArray<AActor*> SpawnedNPCs;

	UPROPERTY()
	AActor* LastSpawnPoint = nullptr;
	UPROPERTY()
	AActor* LastExitLocation = nullptr;


	float PassedTime = 0.0f;
	float NextSpawnTime = 0.0f;

	float SelectShopTime = 0.0f;
	float NextSelectShopTime = 5.0f;
	
	
	FTimerHandle DirectorTimerHandle;
	void DirectorTimer();


	void SelectRandomUniqueShopNPC();
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
};


