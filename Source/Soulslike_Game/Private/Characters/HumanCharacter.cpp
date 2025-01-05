// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HumanCharacter.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystemInterface.h"

AHumanCharacter::AHumanCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHumanCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PlayerController->GetLocalPlayer()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				Subsystem->AddMappingContext(InputContext, 0);
			}
		}
	}
}

void AHumanCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHumanCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AHumanCharacter::InteractWithInteractable);
	}
}

void AHumanCharacter::SetInteractable(TScriptInterface<IInteractable> Interactable)
{
	UE_LOG(LogTemp, Warning, TEXT("AHumanCharacter::SetInteractable"));
	// CurrentInteractable = Interactable;
}

void AHumanCharacter::InteractWithInteractable()
{
	// if (!CurrentInteractable)
	// 	return;
	//
	// CurrentInteractable->InteractWith();
}




