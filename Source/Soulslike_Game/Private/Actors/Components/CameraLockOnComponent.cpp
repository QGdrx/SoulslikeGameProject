// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Components/CameraLockOnComponent.h"

#include "Components/SphereComponent.h"
#include "Interfaces/HealthBarInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utils/CameraUtils.h"

UCameraLockOnComponent::UCameraLockOnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UCameraLockOnComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCameraLockOnComponent::GetValidTargets(TArray<USphereComponent*>& TargetSpheres)
{
	const AActor* Owner = GetOwner();
	const FVector Location = Owner->GetActorLocation();
	const UObject* World = GetWorld();
	TArray<FHitResult> OutHits;
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(World, Location, Location, LockOnRadius, ObjectTypeQuery, false,
		ActorsToIgnore, EDrawDebugTrace::ForDuration, OutHits, true);

	for (FHitResult Result : OutHits)
	{
		if (!Result.GetActor()->Implements<UHealthBarInterface>())
			continue;

		USphereComponent* Sphere = Cast<USphereComponent>(Result.GetComponent());
		if (!Sphere)
			continue;

		if (TargetSpheres.Contains(Sphere))
			continue;

		TargetSpheres.Add(Sphere);
	}
}

USphereComponent* UCameraLockOnComponent::GetBestTargets(TArray<USphereComponent*>& ValidTargets)
{
	const UObject* World = GetWorld();
	const FVector Location = GetOwner()->GetActorLocation();
	const APlayerCameraManager* Camera = UCameraUtils::GetCameraManager(World);
	const FVector Start = Camera->GetCameraLocation();
	FVector CameraForward = Camera->GetActorForwardVector();
	FHitResult OutHit;

	USphereComponent* CurrentTarget = nullptr;
	
	float CurrentDistance = 0.f;
	
	for (USphereComponent* Sphere : ValidTargets)
	{
		const FVector End = Sphere->GetComponentLocation();

		// Ignore the sphere if we hit anything
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel))
			continue;

		if (UCameraUtils::IsCloserFromScreen(CameraForward, Location, Sphere->GetOwner()->GetActorLocation(), CurrentDistance))
		{
			CurrentTarget = Sphere;
		}
	}

	return CurrentTarget;
}

void UCameraLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCameraLockOnComponent::ToggleLock()
{
	if (IsLocked())
		LockOff();
	else
		LockOn();
}

void UCameraLockOnComponent::HideTargetHealthBar() const
{
	if (!Target)
		return;

	AActor* Owner = Target->GetOwner();
	
	if (Owner->Implements<UHealthBarInterface>())
		IHealthBarInterface::Execute_HideHealthBar(Owner);
}

float UCameraLockOnComponent::GetDistance() const
{
	const FVector Location = GetOwner()->GetActorLocation();
	const FVector TargetLocation = Target->GetOwner()->GetActorLocation();

	return UKismetMathLibrary::Vector_Distance(Location, TargetLocation);
}

bool UCameraLockOnComponent::IsTargetInRange() const
{
	return GetDistance() <= LockOnRadius;
}

USphereComponent* UCameraLockOnComponent::GetNextTarget(bool IsLeftCheck)
{
	TArray<USphereComponent*> PossibleTargets;
	GetValidTargets(PossibleTargets);

	const UObject* World = GetWorld();
	const FVector Location = GetOwner()->GetActorLocation();
	const APlayerCameraManager* Camera = UCameraUtils::GetCameraManager(World);
	const FVector Start = Camera->GetCameraLocation();
	FVector CameraForward = Camera->GetActorForwardVector();
	FVector CameraRight = Camera->GetActorRightVector();
	USphereComponent* CurrentTarget = Target;
	
	float CurrentDistance {0.f};
	FHitResult OutHit;

	GEngine->AddOnScreenDebugMessage(1, 5, FColor::Red, FString::Printf(TEXT("Targets Count: %i"), PossibleTargets.Num()) );

	for (USphereComponent* Sphere : PossibleTargets)
	{
		if (Target == Sphere)
			continue;
		
		const FVector End = Sphere->GetComponentLocation();

		// Only check against the given object type to get the hit normal required to know the direction
		//	we should move the camera to
		if (!GetWorld()->LineTraceSingleByObjectType(OutHit, Start, End, ObjectTypeQuery))
			continue;

		float DotProduct = UKismetMathLibrary::Dot_VectorVector(OutHit.Normal, CameraRight);
		DotProduct = UKismetMathLibrary::DegAsin(DotProduct);

		// This doesn't take into account verticality
		if (IsLeftCheck)
		{
			if (DotProduct < 0)
				continue;
			
			if (UCameraUtils::IsCloserFromScreen(CameraForward, Location, Sphere->GetComponentLocation(), CurrentDistance))
			{
				CurrentTarget = Sphere;
			}
		}
		else
		{
			if (DotProduct > 0)
				continue;

			if (UCameraUtils::IsCloserFromScreen(CameraForward, Location, Sphere->GetComponentLocation(), CurrentDistance))
			{
				CurrentTarget = Sphere;
			}
		}
	}

	return CurrentTarget;
}

void UCameraLockOnComponent::OnTimerInternal()
{
	if (!IsTargetInRange())
	{
		LockOff();
		return;
	}

	AActor* Owner = GetOwner();
	const APawn* Pawn = CastChecked<APawn>(Owner);

	AController* Controller = Pawn->GetController();

	OnTimer(Controller);
}

void UCameraLockOnComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// If the Actor is destroyed, we still want to be sure that the timer is cleared
	GetWorld()->GetTimerManager().ClearTimer(LockOnTimerHandle);
}

void UCameraLockOnComponent::ShowTargetHealthBar() const
{
	if (!Target)
		return;
	
	AActor* Owner = Target->GetOwner();

	if (Owner->Implements<UHealthBarInterface>())
		IHealthBarInterface::Execute_ShowHealthBar(Owner);
}

void UCameraLockOnComponent::LockOn()
{
	TArray<USphereComponent*> PossibleTargets;
	GetValidTargets(PossibleTargets);

	HideTargetHealthBar();
	Target = GetBestTargets(PossibleTargets);
	ShowTargetHealthBar();

	if (!Target)
	{
		OnTargetNotFound();
	}
	else
	{
		OnTargetSelected(Target);

		const UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(LockOnTimerHandle, this, &UCameraLockOnComponent::OnTimerInternal,
			UGameplayStatics::GetWorldDeltaSeconds(World), true);
	}
}

void UCameraLockOnComponent::LockOff()
{
	HideTargetHealthBar();
	GetWorld()->GetTimerManager().ClearTimer(LockOnTimerHandle);
	Target = nullptr;
	OnTargetDeselected();
}

bool UCameraLockOnComponent::IsLocked() const
{
	return Target != NULL;
}

void UCameraLockOnComponent::SwitchTarget(const bool LeftCheck, USphereComponent*& OutTarget)
{
	if (!Target)
		return;

	OutTarget = GetNextTarget(LeftCheck);
	if (OutTarget)
	{
		HideTargetHealthBar();
		Target = OutTarget;
		ShowTargetHealthBar();
	}
}

