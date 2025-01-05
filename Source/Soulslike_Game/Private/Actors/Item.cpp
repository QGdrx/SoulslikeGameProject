// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Item.h"

#include "Interfaces/Interact.h"


AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	SetupCallbacks();
}

void AItem::SetupCallbacks()
{
	OnActorBeginOverlap.AddDynamic(this, &AItem::OnBeginOverlap);
}

void AItem::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("AItem::OnBeginOverlap: %s | %s"), *OverlappedActor->GetName(), *OtherActor->GetName());
	if (OtherActor->GetClass()->ImplementsInterface(UInteract::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("AItem::OnBeginOverlap 2"));
		if (IInteract* InteractCharacter = Cast<IInteract>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("AItem::OnBeginOverlap 3"));
			InteractCharacter->SetInteractable(this);
		}
	}
}

void AItem::InteractWith()
{
	GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Blue, TEXT("Interacting"));
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

