// Fill out your copyright notice in the Description page of Project Settings.

#include "NPCDirectorSubsystem.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/Character.h"
#include "UniqueNPC/AI/Actors/NPCWaitLine.h"
#include "UniqueNPC/Interface/NPCWaitLineInterface.h"


bool UNPCDirectorSubsystem::AddSpawnPoint(AActor* SpawnPoint)
{
	return SpawnPoints.AddUnique(SpawnPoint) != INDEX_NONE;
}


bool UNPCDirectorSubsystem::AddExitLocation(AActor* ExitLocation)
{
	return ExitLocations.AddUnique(ExitLocation) != INDEX_NONE;
}

bool UNPCDirectorSubsystem::AddWaitLine(ANPCWaitLine* WaitLine)
{
	return WaitLines.AddUnique(WaitLine) != INDEX_NONE;
}

void UNPCDirectorSubsystem::RequestNPCSpawn(bool bControlNPCLimits)
{
	if(bControlNPCLimits && SpawnedNPCs.Num() >= Settings->TotalNPCs)
	{
		UE_LOG(LogTemp, Log, TEXT("NPC limit reached"));
		return;
	}
	
	// Check if we have any spawn points
	if(SpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No spawn points available"));
		return;
	}

	// Check if we have any exit locations
	if(ExitLocations.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No exit locations available"));
		return;
	}

	// Check if we have a valid NPC class
	if(Settings->NPCClass == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No NPC class set"));
		return;
	}
	
	// Get a random spawn point
	AActor* SpawnPoint = GetRandomSpawnPoint();

	// Get a random exit location
	AActor* ExitLocation = GetRandomExitLocation();

	if (SpawnPoint && ExitLocation)
	{
		// Spawn the NPC
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Owner = GetWorld()->GetGameState();
		SpawnParams.Instigator = nullptr;
		SpawnParams.bNoFail = true;

		
		ACharacter* NPC = GetWorld()->SpawnActor<ACharacter>(Settings->NPCClass, SpawnPoint->GetActorLocation() , FRotator::ZeroRotator, SpawnParams);
		SpawnedNPCs.Add(NPC);
	}
}


void UNPCDirectorSubsystem::RequestDeleteNPC(ACharacter* NPC)
{
	if (SpawnedNPCs.Find(NPC) != INDEX_NONE)
	{
		SpawnedNPCs.Remove(NPC);
		NPC->Destroy();
	}
}


AActor* UNPCDirectorSubsystem::GetRandomSpawnPoint()
{
	AActor* SpawnPoint = nullptr;

	if (SpawnPoints.Num() == 0)
		return nullptr;
	
	if  (SpawnPoints.Num() == 1)
		return SpawnPoints[0];
	
	while (SpawnPoint == nullptr)
	{
		AActor* LocalSpawnPoint = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
		if (LocalSpawnPoint != LastSpawnPoint)
		{
			LastSpawnPoint = LocalSpawnPoint;
			SpawnPoint = LocalSpawnPoint;
			break;
		}
	}
	return LastSpawnPoint;
}


AActor* UNPCDirectorSubsystem::GetRandomExitLocation()
{
	AActor* ExitActor = nullptr;

	if (ExitLocations.Num() == 0)
		return nullptr;
	
	if  (ExitLocations.Num() == 1)
		return ExitLocations[0];
	
	while (ExitActor == nullptr)
	{
		AActor* LocalExitActor = ExitLocations[FMath::RandRange(0, ExitLocations.Num() - 1)];
		if (LocalExitActor != LastExitLocation)
		{
			LastExitLocation = LocalExitActor;
			ExitActor = LocalExitActor;
			break;
		}
	}
	return LastExitLocation;
}


void UNPCDirectorSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	// Load the settings
	Settings = GetMutableDefault<UNPCSettings>();
	NextSpawnTime = FMath::RandRange(Settings->SpawnRate.MinDuration, Settings->SpawnRate.MaxDuration);
	
	if (Settings->bSpawnAllAtOnce)
	{
		FTimerHandle SpawnAllTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(SpawnAllTimerHandle, [this]
		{
			for (int32 i = 0; i < Settings->TotalNPCs; i++)
			{
				RequestNPCSpawn(true);
			}
		}, 0.1, false);
	}
	
	GetWorld()->GetTimerManager().SetTimer(DirectorTimerHandle, this, &UNPCDirectorSubsystem::DirectorTimer, 0.1, true);
	
}


void UNPCDirectorSubsystem::DirectorTimer()
{
	PassedTime += 0.1f;
	if (PassedTime >= NextSpawnTime)
	{
		RequestNPCSpawn(true);
		PassedTime = 0.0f;
		NextSpawnTime = FMath::RandRange(Settings->SpawnRate.MinDuration, Settings->SpawnRate.MaxDuration);
	}

	SelectShopTime += 0.1f;
	if (SelectShopTime >= NextSelectShopTime)
	{
		SelectShopTime = 0.0f;
		SelectRandomUniqueShopNPC();
	}
}

void UNPCDirectorSubsystem::SelectRandomUniqueShopNPC()
{
	AActor* RandomNPC = nullptr;
	for (auto NPC : SpawnedNPCs)
	{
		if (const auto WaitLineInterface = Cast<INPCWaitLineInterface>(NPC))
		{
			if (!WaitLineInterface->Execute_IsShopping(NPC))
			{
				RandomNPC = NPC;
				break;
			}
		}
	}
	if (RandomNPC)
	{
		const auto RandomWaitLine = WaitLines[FMath::RandRange(0, WaitLines.Num() - 1)];

		if (const auto WaitLineInterface = Cast<INPCWaitLineInterface>(RandomNPC))
		{
			WaitLineInterface->Execute_RequestWaitLine(RandomNPC, RandomWaitLine);
		}
	}
}
