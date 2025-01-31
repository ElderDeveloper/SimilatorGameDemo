// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCPlayerCharacter.generated.h"

class UNPCInteractionComponent;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionComponentChanged, UNPCInteractionComponent*, InteractionComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractedWithNPC, AActor*, InteractedNPC);

UCLASS(config=Game)
class UNIQUENPC_API ANPCPlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	ANPCPlayerCharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MouseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	FOnInteractionComponentChanged OnInteractionComponentChanged;
	
	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = Interaction)
 	FOnInteractedWithNPC OnInteractedWithNPC;

	UFUNCTION( BlueprintCallable, Category = Movement)
	void DisablePlayerMovement();

	UFUNCTION( BlueprintCallable, Category = Movement)
	void EnablePlayerMovement();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UNPCInteractionComponent* InteractionComponent;

	void FindInteractionComponent();

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Mouse(const FInputActionValue& Value);
	void Inventory(const FInputActionValue& Value);

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
public:

	UFUNCTION(BlueprintPure, Category = "Interaction")
	FORCEINLINE UNPCInteractionComponent* GetInteractionComponent() const { return InteractionComponent; }
	
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

