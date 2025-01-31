// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCSpawnPoint.h"

#include "Components/BillboardComponent.h"
#include "UniqueNPC/Libraries/NPCGameplayLibrary.h"
#include "UniqueNPC/Subsystem/NPCDirectorSubsystem.h"


// Sets default values
ANPCSpawnPoint::ANPCSpawnPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnPointIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("SpawnPointIcon"));
	RootComponent = SpawnPointIcon;
}

// Called when the game starts or when spawned
void ANPCSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	if (const auto Director = UNPCGameplayLibrary::GetNPCDirectorSubsystem(GetWorld()))
	{
		Director->AddSpawnPoint(this);
	}
	
}

