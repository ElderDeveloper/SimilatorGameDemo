// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NPCInteractionComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFoundInteractNPC, AActor*, InteractableNPC);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLostInteractNPC, AActor*, InteractableNPC);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UNIQUENPC_API UNPCInteractionComponent : public UArrowComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UNPCInteractionComponent();

	// Trace Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Settings")
	bool bTraceStartFromBeginPlay = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Settings")
	float TraceDistance = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Settings")
	float TraceRadius = 25.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Settings")
	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Settings")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType = EDrawDebugTrace::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Settings")
	float TraceDebugDuration = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Settings")
	bool bTraceComplex = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace Settings")
	bool bTraceMulti = false;

	
	UPROPERTY(BlueprintAssignable)
	FOnFoundInteractNPC OnFoundInteractNPC;

	UPROPERTY(BlueprintAssignable)
	FOnLostInteractNPC OnLostInteractNPC;

	UFUNCTION(BlueprintCallable)
	bool RequestInteract();

	UFUNCTION(BlueprintCallable)
	void RequestStartTrace();

	UFUNCTION(BlueprintCallable)
	void RequestStopTrace();

	UFUNCTION(BlueprintPure)
	bool IsLastInteractedActorNPC() const;
	
protected:
	
	UPROPERTY()
	AActor* LastFoundInteractableNPC = nullptr;

	UPROPERTY()
	AActor* LastInteractedNPC = nullptr;
	
	bool bIsTracing = false;
	
	void FindInteractableNPCs();
	void CheckSingleTrace(const FHitResult& HitResult);
	void CheckMultiTrace(const TArray<FHitResult>& HitResults);
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

public:

	UFUNCTION(BlueprintPure)
	AActor* GetLastFoundInteractableNPC() const { return LastFoundInteractableNPC; }

	UFUNCTION(BlueprintPure)
	AActor* GetLastInteractedNPC() const { return LastInteractedNPC; }
};
