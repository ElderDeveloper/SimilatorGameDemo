// Fill out your copyright notice in the Description page of Project Settings.

#include "NPCPlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "NPCGameplayPC.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PawnMovementComponent.h"
#include "UniqueNPC/AI/NPCCharacter.h"
#include "UniqueNPC/Components/NPCInteractionComponent.h"
#include "UniqueNPC/Libraries/NPCGameplayLibrary.h"
#include "UniqueNPC/Subsystem/NPCPlayerInventorySubsystem.h"
#include "UniqueNPC/UI/Hotbar/NPCHotbarItemHolder.h"
#include "UniqueNPC/UI/Hotbar/NPCHotbarWidget.h"

DEFINE_LOG_CATEGORY(LogPlayerCharacter);

//////////////////////////////////////////////////////////////////////////
// ANPCPlayerCharacter

ANPCPlayerCharacter::ANPCPlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
	

}

void ANPCPlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	FindInteractionComponent();

}



void ANPCPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANPCPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANPCPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ANPCPlayerCharacter::Interact);
		EnhancedInputComponent->BindAction(MouseAction, ETriggerEvent::Triggered, this, &ANPCPlayerCharacter::Mouse);
		EnhancedInputComponent->BindAction(InventoryAction, ETriggerEvent::Triggered, this, &ANPCPlayerCharacter::Inventory);
	}
	else
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ANPCPlayerCharacter::DisablePlayerMovement()
{
	GetMovementComponent()->SetComponentTickEnabled( false);
}

void ANPCPlayerCharacter::EnablePlayerMovement()
{
	GetMovementComponent()->SetComponentTickEnabled(true);
}

void ANPCPlayerCharacter::FindInteractionComponent()
{
	if (const auto InteractComp = FindComponentByClass<UNPCInteractionComponent>())
	{
		InteractionComponent = InteractComp;
		OnInteractionComponentChanged.Broadcast(InteractionComponent);
	}
	else
	{
		UE_LOG(LogPlayerCharacter, Error, TEXT("'%s' Failed to find an NPC Interaction Component! Be sure to add it in Blueprints!"), *GetNameSafe(this));
	}
}

void ANPCPlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ANPCPlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANPCPlayerCharacter::Interact(const FInputActionValue& Value)
{
	if (InteractionComponent)
	{
		if (InteractionComponent->RequestInteract())
		{
			UE_LOG(LogPlayerCharacter, Log, TEXT("Interacted with NPC!"));
			OnInteractedWithNPC.Broadcast(InteractionComponent->GetLastInteractedNPC());
			if (InteractionComponent->IsLastInteractedActorNPC())
			{
				DisablePlayerMovement();
			}
		}
	}
}

void ANPCPlayerCharacter::Mouse(const FInputActionValue& Value)
{
	if (const auto PC = Cast<APlayerController>(GetController()))
	{
		if (PC->bShowMouseCursor)
		{
			PC->bShowMouseCursor = false;
			PC->SetInputMode(FInputModeGameOnly());
		}
		else
		{
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeGameAndUI());
		}
	}
}


void ANPCPlayerCharacter::Inventory(const FInputActionValue& Value)
{
	float ActionIndex = Value.Get<float>();

	if (const auto InventorySubsystem = UNPCGameplayLibrary::GetNPCPlayerInventorySubsystem(GetWorld()))
	{
		FInventoryItem Item;
		if (InventorySubsystem->GetItemAtIndex(ActionIndex - 1,Item))
		{
			if (const auto NPCPC = Cast<ANPCGameplayPC>(GetController()))
			{
				NPCPC->GetHotbarWidget()->ItemHolders[ActionIndex - 1]->HighlightItem(true);
			}
		}
	}
}

