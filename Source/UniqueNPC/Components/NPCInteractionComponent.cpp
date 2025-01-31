// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCInteractionComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UniqueNPC/Interface/NPCInteractionInterface.h"


UNPCInteractionComponent::UNPCInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	ArrowColor = FColor::Emerald;
}


bool UNPCInteractionComponent::RequestInteract()
{
	if (LastFoundInteractableNPC)
	{
		if (INPCInteractionInterface* NPCInterface = Cast<INPCInteractionInterface>(LastFoundInteractableNPC))
		{
			if (NPCInterface->Execute_CanInteract(LastFoundInteractableNPC,Cast<ACharacter>(GetOwner())))
			{
				if (NPCInterface->Execute_OnInteract(LastFoundInteractableNPC, Cast<ACharacter>(GetOwner())))
				{
					LastInteractedNPC = LastFoundInteractableNPC;
					return true;
				}
			}
		}
	}
	UE_LOG(LogTemp, Log , TEXT("Nothing to interact with!"));
	return false;

}


void UNPCInteractionComponent::RequestStartTrace()
{
	bIsTracing = true;
}


void UNPCInteractionComponent::RequestStopTrace()
{
	bIsTracing = false;
	if (LastFoundInteractableNPC)
	{
		OnLostInteractNPC.Broadcast(LastFoundInteractableNPC);
		LastFoundInteractableNPC = nullptr;
	}
}

bool UNPCInteractionComponent::IsLastInteractedActorNPC() const
{
	if (LastInteractedNPC)
	{
		if (INPCInteractionInterface* NPCInterface = Cast<INPCInteractionInterface>(LastInteractedNPC))
		{
			return NPCInterface->Execute_IsNPCCharacter(LastInteractedNPC);
		}
	}
	return false;
}


void UNPCInteractionComponent::FindInteractableNPCs()
{
	const FVector TraceStart = GetComponentLocation();
	const FVector TraceEnd = TraceStart + GetComponentRotation().Vector() * TraceDistance;
	TArray<AActor*> IgnoredActors = { GetOwner() };

	if (bTraceMulti)
	{
		TArray<FHitResult> HitResults;
		UKismetSystemLibrary::SphereTraceMulti(this,
			TraceStart,
			TraceEnd,
			TraceRadius,
			TraceTypeQuery,
			bTraceComplex,
			IgnoredActors,
			DrawDebugType,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			TraceDebugDuration
			);
		CheckMultiTrace(HitResults);
	}
	else
	{
		FHitResult HitResult;
		UKismetSystemLibrary::SphereTraceSingle(this,
			TraceStart,
			TraceEnd
			, TraceRadius,
			TraceTypeQuery,
			bTraceComplex,
			IgnoredActors,
			DrawDebugType, HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			TraceDebugDuration
			);
		CheckSingleTrace(HitResult);
	}

}


void UNPCInteractionComponent::CheckSingleTrace(const FHitResult& HitResult)
{
	if (HitResult.GetActor())
	{
		if (INPCInteractionInterface* NPCInterface = Cast<INPCInteractionInterface>(HitResult.GetActor()))
		{
			if (HitResult.GetActor() != LastFoundInteractableNPC)
			{
				LastFoundInteractableNPC = HitResult.GetActor();
				OnFoundInteractNPC.Broadcast(LastFoundInteractableNPC);
			}
			return;
		}
	}

	if (LastFoundInteractableNPC)
	{
		OnLostInteractNPC.Broadcast(LastFoundInteractableNPC);
		LastFoundInteractableNPC = nullptr;
	}
}


void UNPCInteractionComponent::CheckMultiTrace(const TArray<FHitResult>& HitResults)
{
	if (HitResults.Num() > 0)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (Hit.GetActor())
			{
				if (INPCInteractionInterface* NPCInterface = Cast<INPCInteractionInterface>(Hit.GetActor()))
				{
					if (Hit.GetActor() != LastFoundInteractableNPC)
					{
						LastFoundInteractableNPC = Hit.GetActor();
						OnFoundInteractNPC.Broadcast(LastFoundInteractableNPC);
					}
					return;
				}
			}
		}
		if (LastFoundInteractableNPC)
		{
			OnLostInteractNPC.Broadcast(LastFoundInteractableNPC);
			LastFoundInteractableNPC = nullptr;
		}
	}
}


void UNPCInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (bTraceStartFromBeginPlay)
	{
		 RequestStartTrace();
	}
	
}


void UNPCInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsTracing)
	{
		FindInteractableNPCs();
	}
}

