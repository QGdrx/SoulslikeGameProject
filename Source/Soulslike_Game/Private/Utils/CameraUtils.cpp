// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CameraUtils.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCameraManager* UCameraUtils::GetCameraManager(const UObject* World)
{
	return UGameplayStatics::GetPlayerCameraManager(World, 0);;
}

float UCameraUtils::GetDistanceFromScreenCenter(FVector& CameraForward, const FVector PlayerLocation, const FVector TargetLocation)
{
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, TargetLocation);
	const FVector ForwardVector = UKismetMathLibrary::Conv_RotatorToVector(LookAtRotation);

	return UKismetMathLibrary::Dot_VectorVector(CameraForward, ForwardVector);
}

bool UCameraUtils::IsCloserFromScreen(FVector& CameraForward, const FVector PlayerLocation, const FVector TargetLocation, float& CurrentDistance)
{
	const float DistanceToCenter = GetDistanceFromScreenCenter(CameraForward, PlayerLocation, TargetLocation);

	const bool IsCloser {DistanceToCenter > CurrentDistance};

	if (IsCloser)
		CurrentDistance = DistanceToCenter;
	
	return IsCloser;
}




