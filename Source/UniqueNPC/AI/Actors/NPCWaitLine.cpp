// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCWaitLine.h"

#include "Components/BillboardComponent.h"
#include "UniqueNPC/Interface/NPCWaitLineInterface.h"
#include "UniqueNPC/Libraries/NPCGameplayLibrary.h"
#include "UniqueNPC/Subsystem/NPCDirectorSubsystem.h"


FVector FLineHolder::GetLocation() const
{
	return LineLocation->GetComponentLocation();
}

bool FLineHolder::SetActor(const AActor* NewActor)
{
	if (NewActor)
	{
		if (!IsActor(NewActor))
		{
			Actor = NewActor;
			return true;
		}
	}
	return false;
}

bool FLineHolder::IsActor(const AActor* OtherActor) const
{
	if (!Actor.IsValid())
	{
		 return false;
	}
	return Actor.Get() == OtherActor;
}



// Sets default values
ANPCWaitLine::ANPCWaitLine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}


// Called when the game starts or when spawned
void ANPCWaitLine::BeginPlay()
{
	Super::BeginPlay();

	if (const auto Director = UNPCGameplayLibrary::GetNPCDirectorSubsystem(GetWorld()))
	{
		Director->AddWaitLine(this);
	}
}

bool ANPCWaitLine::IsActorInLine(const AActor* Actor) const
{
	for (const auto& Line : WaitLines)
	{
		if (Line.IsActor(Actor))
		{
			return true;
		}
	}
	return false;
}


void ANPCWaitLine::ConstructWaitLine()
{
	for (const auto WaitLine : WaitLines)
	{
		if (WaitLine.LineLocation)
		{
			WaitLine.LineLocation->DestroyComponent();
		}
		if (WaitLine.Actor)
		{
			WaitLine.Actor->Destroy();
		}
	}

	WaitLines.Empty();
	WaitLines.SetNum(WaitLineSize);

	for (int i = 0; i < WaitLineSize; i++)
	{
		FLineHolder LineHolder = FLineHolder();
		UBillboardComponent* WaitLineLocation = NewObject<UBillboardComponent>(this, *FString::Printf(TEXT("WaitLineLocation_%d"), i));
		FinishAndRegisterComponent(WaitLineLocation);
		WaitLineLocation->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
		WaitLineLocation->SetSprite(WaitLineTexture);
		WaitLineLocation->SetRelativeLocation(FVector(i * WaitLineSpacing, 0.0f, 0.0f));
		LineHolder.LineLocation = WaitLineLocation;
		WaitLines[i] = LineHolder;
	}
}



bool ANPCWaitLine::RequestLineForActor(AActor* Actor , FVector& OutLocation, uint8& OutLineIndex)
{
	if (IsLineFull())
	{
		return false;
	}

	if (IsActorInLine(Actor))
	{
		return false;
	}

	uint8 EmptyIndex = GetFirstEmptyLineIndex();
	if (EmptyIndex != INDEX_NONE)
	{
		WaitLines[EmptyIndex].Actor = Actor;
		OutLocation = WaitLines[EmptyIndex].GetLocation();
		OutLineIndex = EmptyIndex;
		return true;
	}
	return false;
}


bool ANPCWaitLine::LeaveLine(AActor* Actor)
{
	for (int i = 0; i < WaitLines.Num(); i++)
	{
		if (WaitLines[i].IsActor(Actor))
		{
			WaitLines[i].Clear();
			OnActorLeftLine();
			return true;
		}
	}
	return false;
}


void ANPCWaitLine::OnActorLeftLine()
{
	// Move all actors to the front of the line if there are empty spaces

	bool Searching = true;
	while (Searching)
	{
		Searching = false;
		for (int i = 0; i < WaitLines.Num() - 1; i++)
		{
			if (WaitLines[i].IsEmpty() && WaitLines[i + 1].IsActorValid())
			{
				Searching = true;
				WaitLines[i].Actor = WaitLines[i + 1].Actor;
				WaitLines[i + 1].Clear();
			}
		}
	}

	for (const auto& Line : WaitLines)
	{
		if (Line.IsActorValid())
		{
			if (const auto Interface = Cast<INPCWaitLineInterface>(Line.Actor.Get()))
			{
				Interface->Execute_MoveToLine(Line.Actor.Get(), this, Line.GetLocation(), Line.LineIndex);
			}
		}
	}
	
}




bool ANPCWaitLine::IsLineIndexFull(const uint8 LineIndex)
{
	if (WaitLines.IsValidIndex(LineIndex))
	{
		return WaitLines[LineIndex].Actor.IsValid();
	}
	return false;
}


bool ANPCWaitLine::IsLineFull() const
{
	for (const auto& Status : WaitLines)
	{
		if (!Status.Actor.IsValid())
		{
			return false;
		}
	}
	return true;
}


uint8 ANPCWaitLine::GetFirstEmptyLineIndex() const
{
	for (int i = 0; i < WaitLines.Num(); i++)
	{
		if (!WaitLines[i].Actor.IsValid())
		{
			return i;
		}
	}
	return INDEX_NONE;
}
