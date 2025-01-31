// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCExitPoint.h"

#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "UniqueNPC/Interface/NPCInteractionInterface.h"
#include "UniqueNPC/Libraries/NPCGameplayLibrary.h"
#include "UniqueNPC/Subsystem/NPCDirectorSubsystem.h"


// Sets default values
ANPCExitPoint::ANPCExitPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ExitPointIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("ExitPointIcon"));
	RootComponent = ExitPointIcon;

	ExitPointTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("ExitPointTrigger"));
	ExitPointTrigger->SetupAttachment(ExitPointIcon);
	ExitPointTrigger->SetBoxExtent(BoxExtent);
	ExitPointTrigger->SetCollisionResponseToAllChannels(ECR_Overlap);
	
}

void ANPCExitPoint::OnExitPointTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (const auto InteractionInterface = Cast<INPCInteractionInterface>(OtherActor))
	{
		if (const auto NPC = Cast<ACharacter>(OtherActor))
		{
			if (const auto Director = UNPCGameplayLibrary::GetNPCDirectorSubsystem(GetWorld()))
			{
				Director->RequestDeleteNPC(NPC);
			}
		}
	}

}

// Called when the game starts or when spawned
void ANPCExitPoint::BeginPlay()
{
	Super::BeginPlay();

	if (const auto Director = UNPCGameplayLibrary::GetNPCDirectorSubsystem(GetWorld()))
	{
		Director->AddExitLocation(this);
	}

	ExitPointTrigger->SetBoxExtent(BoxExtent);
	ExitPointTrigger->OnComponentBeginOverlap.AddDynamic(this, &ANPCExitPoint::OnExitPointTriggerBeginOverlap);
	
	
}

