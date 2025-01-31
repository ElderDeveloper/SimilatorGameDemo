// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCItemBase.h"

#include "Components/SphereComponent.h"
#include "UniqueNPC/Libraries/NPCGameplayLibrary.h"
#include "UniqueNPC/Subsystem/NPCPlayerInventorySubsystem.h"


// Sets default values
ANPCItemBase::ANPCItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(MeshComponent);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	
}


void ANPCItemBase::DestroyItem()
{
	Destroy();
}

void ANPCItemBase::InitializeItem(const FInventoryItem& NewItemData)
{
	ItemData = NewItemData;
	if (ItemData.ItemMesh)
	{
		MeshComponent->SetStaticMesh(ItemData.ItemMesh);
	}
}

bool ANPCItemBase::OnInteract_Implementation(ACharacter* InteractingCharacter)
{
	if (const auto InventorySubsystem = UNPCGameplayLibrary::GetNPCPlayerInventorySubsystem(GetWorld()))
	{
		InventorySubsystem->AddItem(ItemData.ItemTag, 1);
		FTimerHandle DestroyTimer;
		GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ANPCItemBase::DestroyItem, 0.f, false);
	}
	return true;
}


void ANPCItemBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			if (bShouldCollectOnOverlap)
			{
				if (const auto InventorySubsystem = UNPCGameplayLibrary::GetNPCPlayerInventorySubsystem(GetWorld()))
				{
					InventorySubsystem->AddItem(ItemData.ItemTag, 1);
					Destroy();
				}
			}
		}
	}
}


void ANPCItemBase::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			
		}
	}
}


// Called when the game starts or when spawned
void ANPCItemBase::BeginPlay()
{
	Super::BeginPlay();

	const auto ItemDetails = UNPCGameplayLibrary::GetItemDetails(ItemData.ItemTag);
	ItemData = ItemDetails;
	if (ItemDetails.ItemMesh)
	{
		MeshComponent->SetStaticMesh(ItemData.ItemMesh);
	}

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ANPCItemBase::OnOverlapBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ANPCItemBase::OnOverlapEnd);
}



