// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCWaitLine.generated.h"

class ANPCWaitLine;
class UBillboardComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLineActorLeft, ANPCWaitLine*, WaitLine, uint8, LineIndex);

USTRUCT(BlueprintType)
struct FLineHolder
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UBillboardComponent* LineLocation;

	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<AActor> Actor;

	UPROPERTY(BlueprintReadWrite)
	uint8 LineIndex;

	FVector GetLocation() const;
	bool IsEmpty() const { return !Actor.IsValid(); }
	bool IsActorValid() const { return Actor.IsValid(); }
	void Clear() { Actor.Reset(); }
	bool SetActor(const AActor* NewActor);
	bool IsActor(const AActor* OtherActor) const;
};

UCLASS()
class UNIQUENPC_API ANPCWaitLine : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANPCWaitLine();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WaitLine")
	USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaitLine")
	uint8 WaitLineSize = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaitLine")
	float WaitLineSpacing = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaitLine")
	UTexture2D* WaitLineTexture;

	UPROPERTY(BlueprintAssignable)
	FOnLineActorLeft OnFirstLineLeft;

	UFUNCTION(BlueprintCallable)
	bool RequestLineForActor(AActor* Actor, FVector& OutLocation, uint8& OutLineIndex);

	UFUNCTION(BlueprintCallable)
	bool LeaveLine(AActor* Actor);

protected:

	UFUNCTION(BlueprintCallable)
	void ConstructWaitLine();

	UPROPERTY()
	TArray<FLineHolder> WaitLines;

	void OnActorLeftLine();
	
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintPure)
	bool IsActorInLine(const AActor* Actor) const;
	
	UFUNCTION(BlueprintPure)
	bool IsLineIndexFull(const uint8 LineIndex);

	UFUNCTION(BlueprintPure)
	bool IsLineFull() const;

	UFUNCTION(BlueprintPure)
	uint8 GetFirstEmptyLineIndex() const;
};
