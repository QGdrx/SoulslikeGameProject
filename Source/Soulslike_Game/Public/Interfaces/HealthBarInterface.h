// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthBarInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UHealthBarInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SOULSLIKE_GAME_API IHealthBarInterface
{
	GENERATED_BODY()

	// Recheck that page because it doesn't work like described on that page
	// https://dev.epicgames.com/documentation/en-us/unreal-engine/interfaces-in-unreal-engine
public:	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health Bar")
	void SetHealthBarVisibility(bool Visible);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health Bar")
	void ShowHealthBar();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health Bar")
	void HideHealthBar();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health Bar")
	void UpdateHealthBarRotation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Health Bar")
	void StopHealthBarRotation();
};
