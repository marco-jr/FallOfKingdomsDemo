// Fill out your copyright notice in the Description page of Project Settings.


#include "HandComponent.h"
#include "ItemObject.h"
#include "Components/StaticMeshComponent.h"
#include "HandWidget.h"
#include "CharacterController.h"

// Sets default values for this component's properties
UHandComponent::UHandComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bMainHand = false;
	ArmLength = 65;
}


void UHandComponent::AddItemObject(AItemObject* ItemObjectReference)
{
	if (ItemObjectReference != nullptr)
	{
		ItemObject = ItemObjectReference;
		ItemObject->SetActorEnableCollision(false);
		UStaticMeshComponent* _StaticMeshReference = Cast<UStaticMeshComponent>(ItemObject->GetRootComponent());
		_StaticMeshReference->SetSimulatePhysics(false);
		ItemObject->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		HandWidget->AddItemObject(ItemObjectReference->ItemObjectWidget);
	}
}

void UHandComponent::RemoveItemObject(bool bSimulatePhysics)
{
	if (ItemObject != nullptr)
	{
		ItemObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		ItemObject->SetActorEnableCollision(true);
		if (bSimulatePhysics)
		{
			UStaticMeshComponent* _StaticMeshReference = Cast<UStaticMeshComponent>(ItemObject->GetRootComponent());
			_StaticMeshReference->SetSimulatePhysics(true);
		}
		ItemObject = nullptr;
		HandWidget->RemoveItemObject();
	}
}

void UHandComponent::UseItemObject()
{
	// TODO: CALL USE ITEM ON ITEMOBJECT
}

// Called when the game starts
void UHandComponent::BeginPlay()
{
	Super::BeginPlay();

	if (HandWidgetClass)
	{
		if (ACharacterController* _ControllerReference = Cast<ACharacterController>(GetWorld()->GetFirstPlayerController()))
		{
			HandWidget = CreateWidget<UHandWidget>(_ControllerReference, HandWidgetClass);
			HandWidget->SetOwnerHand(this);
			HandWidget->AddToViewport(1);
		}
	}
}