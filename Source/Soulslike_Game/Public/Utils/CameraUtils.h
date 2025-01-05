// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CameraUtils.generated.h"

class UCameraComponent;
/**
 * 
 */
UCLASS()
class SOULSLIKE_GAME_API UCameraUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static APlayerCameraManager* GetCameraManager(const UObject* World);

	UFUNCTION(BlueprintCallable)
	static float GetDistanceFromScreenCenter(FVector& CameraForward, FVector PlayerLocation, FVector TargetLocation);

	/**
	 * Calculate the distance of the target to the screen center.
	 * @param CameraForward The camera looking direction.
	 * @param PlayerLocation The location of the player.
	 * @param TargetLocation The target location.
	 * @param CurrentDistance The smallest distance between CurrentDistance and the target location from the screen center.
	 * @return If the target is closer to the center of the screen than `CurrentDistance`.
	 */
	UFUNCTION(BlueprintCallable)
	static bool IsCloserFromScreen(FVector& CameraForward, FVector PlayerLocation, FVector TargetLocation, float& CurrentDistance);
};
