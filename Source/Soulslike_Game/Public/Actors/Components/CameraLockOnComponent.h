// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraLockOnComponent.generated.h"


class USphereComponent;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOULSLIKE_GAME_API UCameraLockOnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	/* Base Data */
	UCameraLockOnComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	
public: /* Lock On */
	UFUNCTION(BlueprintCallable)
	void ToggleLock();

	UFUNCTION(BlueprintCallable)
	void LockOn();

	UFUNCTION(BlueprintCallable)
	void LockOff();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsLocked() const;

	UFUNCTION(BlueprintCallable, meta = (LeftCheck = true))
	void SwitchTarget(bool LeftCheck, USphereComponent*& OutTarget);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On|Raycasts")
	TArray<TObjectPtr<AActor>> ActorsToIgnore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lock On")
	float LockOnRadius {2000};

	UPROPERTY(EditDefaultsOnly, Category = "Lock On|Raycasts")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Lock On|Raycasts")
	TEnumAsByte<ECollisionChannel> TraceChannel {ECC_Visibility};

	UPROPERTY(BlueprintReadWrite, Category = "Lock On")
	TObjectPtr<USphereComponent> Target;

private:
	void GetValidTargets(TArray<USphereComponent*>& TargetSpheres);
	USphereComponent* GetBestTargets(TArray<USphereComponent*>& ValidTargets);
	
	void ShowTargetHealthBar() const;
	void HideTargetHealthBar() const;
	bool IsTargetInRange() const;
	
	USphereComponent* GetNextTarget(bool IsLeftCheck = true);

	FTimerHandle LockOnTimerHandle;

	void OnTimerInternal();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	
public: /* Callbacks */
	/**
	 * Called when a target is selected by the component.
	 * @param SelectedTarget The target locked onto.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Lock On|Delegates")
	void OnTargetSelected(USphereComponent* SelectedTarget);

	/**	 Called when the camera locks off from a target. 	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Lock On|Delegates")
	void OnTargetDeselected();

	/**	 Called when no target is found when trying to lock on.	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Lock On|Delegates")
	void OnTargetNotFound();

	/**
	 * Called every delta time seconds when a target is locked.
	 * @param Controller The controller associated with this component.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Lock On|Delegates")
	void OnTimer(AController* Controller);

	
public: /* Utils */
	UFUNCTION(BlueprintCallable, Category = "Lock On|Utils", meta = (ReturnDisplayName = "Distance"))
	float GetDistance() const;
};
