// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"
#include "LockComponent.h"
#include "InteractableComponent.h"
#include "PlayerCharacter.h"
#include "HandComponent.h"
#include "ActionMessageWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	DoorStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	LockComponent = CreateDefaultSubobject<ULockComponent>(TEXT("LockComponent"));

	RootComponent = SceneComponent;
	DoorStaticMesh->SetupAttachment(RootComponent);
	InteractableComponent->SetupAttachment(DoorStaticMesh);

	DoorRotateAmount = 3;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	GetInteractableComponent()->InteractDelegate.AddDynamic(this, &ADoor::Interacted);
}

void ADoor::OnConstruction(const FTransform& Transform)
{
	if (DoorMesh != nullptr)
	{
		DoorStaticMesh->SetStaticMesh(DoorMesh);
	}

	float _DoorYaw = GetIsClosing() ? 0 : GetIsInverseDoor() ? -90 : 90;
	GetDoorStaticMesh()->SetRelativeRotation(FRotator(0, _DoorYaw, 0));
}

void ADoor::Interacted(UHandComponent* HandReference)
{
	if (GetLockComponent()->GetIsLocked())
	{
		UGameplayStatics::PlaySoundAtLocation(this, GetLockedDoorSound(), GetInteractableComponent()->GetComponentLocation());
		if (APlayerCharacter* _CharacterReference = Cast<APlayerCharacter>(HandReference->GetOwner()))
		{
			_CharacterReference->ShowMessage(GetLockedMessage(), EMessageType::Warning);
		}
	}
	else
	{
		bClosing = !bClosing;
		GetDoorStaticMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UGameplayStatics::PlaySoundAtLocation(this, GetIsClosing() ? GetOpenDoorSound() : GetCloseDoorSound(), GetInteractableComponent()->GetComponentLocation());
		GetWorld()->GetTimerManager().SetTimer(DoorRotateHandle, this, &ADoor::DoorRotate, 0.015f, true);
	}
}

void ADoor::DoorRotate()
{
	float _RotateValue = GetIsClosing() && !GetIsInverseDoor() || !GetIsClosing() && GetIsInverseDoor() ? GetDoorRotateAmount() * -1 : GetDoorRotateAmount();

	GetDoorStaticMesh()->AddRelativeRotation(FRotator(0, _RotateValue, 0), true);

	float _DoorYaw = GetDoorStaticMesh()->GetRelativeRotation().Yaw;
	float _YawComparison = GetIsClosing() ? 0 : GetIsInverseDoor() ? -90 : 90;

	if (FMath::IsNearlyEqual(_DoorYaw, _YawComparison, GetDoorRotateAmount()))
	{
		GetWorld()->GetTimerManager().ClearTimer(DoorRotateHandle);
		GetDoorStaticMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}
