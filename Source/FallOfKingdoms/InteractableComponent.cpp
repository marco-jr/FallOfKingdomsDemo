// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableComponent.h"
#include "HandComponent.h"

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bHandFreeNeeded = true;
	bAnimateInteraction = true;
	InitSphereRadius(40);
	SetCollisionObjectType(ECC_WorldDynamic);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}


void UInteractableComponent::Interacted(UHandComponent* HandReference)
{
	InteractDelegate.Broadcast(HandReference);
}

// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UpdateCollision(true);
}

void UInteractableComponent::UpdateCollision(bool bCollides)
{
	SetCollisionEnabled(bCollides ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}
