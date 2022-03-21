// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObject.h"
#include "Components/StaticMeshComponent.h"
#include "HandComponent.h"
#include "InteractableComponent.h"
#include "CharacterController.h"
#include "ItemObjectWidget.h"

// Sets default values
AItemObject::AItemObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Object Mesh"));
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("Interactable Component"));

	ObjectMesh->SetupAttachment(RootComponent);
	InteractableComponent->SetupAttachment(ObjectMesh);

	ObjectMesh->SetMassOverrideInKg(TEXT("NAME_None"), GetItemObjectWeight() * 10.f, true);

	InteractableComponent->bHandFreeNeeded = true;
	InteractableComponent->InteractionText = FText::FromString("Pegar");
}

// Called when the game starts or when spawned
void AItemObject::BeginPlay()
{
	Super::BeginPlay();
	
	if (ObjectMesh->GetStaticMesh() != nullptr)
	{
		FVector _MiddleLocation = ObjectMesh->GetStaticMesh()->GetBoundingBox().GetCenter();
		InteractableComponent->SetRelativeLocation(_MiddleLocation);
	}
	InteractableComponent->InteractDelegate.AddDynamic(this, &AItemObject::Interacted);

	if (ACharacterController* _ControllerReference = Cast<ACharacterController>(GetWorld()->GetFirstPlayerController()))
	{
		ItemObjectWidget = CreateWidget<UItemObjectWidget>(_ControllerReference, ItemObjectWidgetClass);
		ItemObjectWidget->SetItemObjectImage(ItemObjectImage);
	}
}

void AItemObject::Interacted(UHandComponent* HandReference)
{
	HandReference->AddItemObject(this);
	InteractableComponent->UpdateCollision(false);
}
