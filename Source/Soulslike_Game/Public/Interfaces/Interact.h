// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Interactable.h"
#include "UObject/Interface.h"
#include "Interact.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UInteract : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOULSLIKE_GAME_API IInteract
{
	GENERATED_BODY()

public:
	// TScriptInterface is required for an interface to reference another
	virtual void SetInteractable(TScriptInterface<IInteractable> Interactable) abstract;
};
