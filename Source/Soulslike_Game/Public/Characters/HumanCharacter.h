// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Interact.h"
#include "Interfaces/Interactable.h"
#include "HumanCharacter.generated.h"

class UCameraLockOnComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class SOULSLIKE_GAME_API AHumanCharacter : public ACharacter , public IInteract
{
	GENERATED_BODY()

public:
	AHumanCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCameraLockOnComponent> CameraLockOn;
	
protected:
	virtual void BeginPlay() override;

/*
 *	Inputs
 */
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	TObjectPtr<UInputMappingContext> InputContext;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	TObjectPtr<UInputAction> InteractAction;

/*
 *	Interaction System
 */
public:	
	virtual void SetInteractable(TScriptInterface<IInteractable> Interactable);

private:
	void InteractWithInteractable();
};
