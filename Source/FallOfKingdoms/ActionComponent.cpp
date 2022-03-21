// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionComponent.h"
#include "GameFramework/Character.h"
#include "HandComponent.h"
#include "CharacterController.h"
#include "ActionMessageWidget.h"
#include "PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActionWidget.h"
#include "FOKFunctionsLibrary.h"

// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bIsHumanoid = true;
	WalkSpeed = 225;
	SprintSpeed = 450;
	Acceleration = 512;
	CurrentAction = ECurrentAction::Ready;
	bCannotMove = false;
	bSprinting = false;
	ExhaustTime = 0.2f;

	{ // DEFAULT ACTIONS STOP MOVEMENT ARRAY
		{ // INTERACT
			DefaultActionsStopMovement.Add(ECurrentAction::Interact);
			DefaultActionsStopMovement.Add(ECurrentAction::Attack);
		}
	}

	{ // DEFAULT ACTIONS DURATION ARRAY
		{ // INTERACT
			DefaultActionsDuration.Add(ECurrentAction::Interact, .6f);
			DefaultActionsDuration.Add(ECurrentAction::Attack, .7f);
		}
	}
}


// Called when the game starts
void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Cast<ACharacter>(GetOwner()))
	{
		OwnerCharacter = Cast<ACharacter>(GetOwner());
		UpdateMovementSpeed();
	}

	if (APlayerCharacter* _CharacterReference = Cast<APlayerCharacter>(GetOwner()))
	{
		ActionWidget = CreateWidget<UActionWidget>(GetWorld()->GetFirstPlayerController(), ActionWidgetClass);
		ActionWidget->AddToViewport(0);
	}
}

void UActionComponent::UpdateMovementSpeed()
{
	if (DefaultActionsStopMovement.Contains(CurrentAction) || bCannotMove == true)
	{
		OwnerCharacter->GetCharacterMovement()->MaxAcceleration = 0;
	}
	else
	{
		OwnerCharacter->GetCharacterMovement()->MaxAcceleration = Acceleration;
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = bSprinting ? SprintSpeed : WalkSpeed;
	}

	bSprinting ?
		GetWorld()->GetTimerManager().SetTimer(SprintConsumptionTimer, this, &UActionComponent::SprintConsumption, true, 1)
		:
		GetWorld()->GetTimerManager().ClearTimer(SprintConsumptionTimer)
		;
}

void UActionComponent::SetCannotMove()
{
	bCannotMove = true;
	UpdateMovementSpeed();
}

void UActionComponent::SetCanMove()
{
	bCannotMove = false;
	UpdateMovementSpeed();
}

void UActionComponent::Attack()
{
	if (GetCanDoActions())
	{
		int _ActionID = 0;
		if (GetIsHumanoid())
		{
			// TODO (GET CORRECT ACTION ID) : _ActionID = UFOKFunctionsLibrary::GetMainHand(GetOwner())->GetItemObject()
		}

		DoAction(ECurrentAction::Attack, _ActionID);
	}
}


void UActionComponent::Sprint()
{
	bSprinting = true;
	UpdateMovementSpeed();
}

void UActionComponent::StopSprint()
{
	bSprinting = false;
	UpdateMovementSpeed();
}

bool UActionComponent::GetCanDoActions(bool bShowBusyMessage)
{
	float _CurrentTime = GetWorld()->GetRealTimeSeconds();
	float _NewActionExhaustClearTime = CurrentActionStartTime + ExhaustTime;

	if (CurrentAction == ECurrentAction::Ready && _CurrentTime >= _NewActionExhaustClearTime)
	{
		return true;
	}
	else
	{
		if (bShowBusyMessage) {
			ACharacterController* _ControllerReference = Cast<ACharacterController>(OwnerCharacter->Controller);
			if (_ControllerReference)
			{
				_ControllerReference->ShowMessage(GetBusyMessage(), EMessageType::Warning);
			}
		}
		return false;
	};
}

void UActionComponent::DoAction(ECurrentAction NewAction, int ActionID, bool bShowActionProgress, float CustomDuration)
{
	GetWorld()->GetTimerManager().ClearTimer(ActionCompleteTimer);
	
	if (CurrentAction != ECurrentAction::Ready && NewAction != ECurrentAction::Ready)
	{
		ActionInterrupted();
	}

	CurrentAction = NewAction;
	CurrentActionID = ActionID;
	CurrentActionStartTime = GetWorld()->GetRealTimeSeconds();

	UpdateMovementSpeed();


	if (CustomDuration > 0)
	{
		CurrentActionDuration = CustomDuration;
		GetWorld()->GetTimerManager().SetTimer(ActionCompleteTimer, this, &UActionComponent::ActionCompleted, CurrentActionDuration, false);
	}
	else if (DefaultActionsDuration.Contains(NewAction))
	{
		CurrentActionDuration = *DefaultActionsDuration.Find(NewAction);
		GetWorld()->GetTimerManager().SetTimer(ActionCompleteTimer, this, &UActionComponent::ActionCompleted, CurrentActionDuration, false);
	}
	else
	{
		CurrentActionDuration = 0;
	}

	if (bShowActionProgress)
	{
		ActionWidget->StartActionProgress(CurrentActionDuration);
	}
}

void UActionComponent::ActionCompleted()
{
	Ready();

	ActionCompletedDelegate.Broadcast(this);
	ActionWidget->StopActionProgress();

	ActionCompletedDelegate.Clear();
	ActionInterruptedDelegate.Clear();
}

void UActionComponent::ActionInterrupted()
{
	ActionInterruptedDelegate.Broadcast(this);
	ActionWidget->StopActionProgress();

	ActionCompletedDelegate.Clear();
	ActionInterruptedDelegate.Clear();
}

void UActionComponent::Ready()
{
	DoAction(ECurrentAction::Ready, 0, false);
}

void UActionComponent::Busy()
{
	DoAction(ECurrentAction::Busy, 0, false);
}

void UActionComponent::SprintConsumption()
{
	// TODO: + VALUE TO FATIGUE
}