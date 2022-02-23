// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	WalkSpeed = 225;
	SprintSpeed = 450;
	Acceleration = 512;

	if (Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner());
	}
}


// Called when the game starts
void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerCharacter)
	{
		SetMovementSpeed(false);
	}
}

void UActionComponent::SetMovementSpeed(bool Sprint)
{
	OwnerCharacter->GetCharacterMovement()->MaxAcceleration = Sprint ? Acceleration * 2 : Acceleration;
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Sprint ? SprintSpeed : WalkSpeed;

	Sprint ?
		GetWorld()->GetTimerManager().SetTimer(SprintConsumptionTimer, this, &UActionComponent::SprintConsumption, true, 1)
		:
		GetWorld()->GetTimerManager().ClearTimer(SprintConsumptionTimer)
		;
}

void UActionComponent::Sprint()
{
	SetMovementSpeed(true);
}

void UActionComponent::StopSprint()
{
	SetMovementSpeed(false);
}

void UActionComponent::SprintConsumption()
{
	// TODO: + VALUE TO FATIGUE
}