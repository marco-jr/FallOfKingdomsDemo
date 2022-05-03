// Fill out your copyright notice in the Description page of Project Settings.


#include "TalkComponent.h"
#include "TalkWidget.h"
#include "CharacterController.h"
#include "FOKFunctionsLibrary.h"
#include "PlayerCharacter.h"
#include "CharacterIdentity.h"
#include "ActionComponent.h"
#include "ActionMessageWidget.h"
#include "Components/BackgroundBlur.h"

// Sets default values for this component's properties
UTalkComponent::UTalkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...

	InitSphereRadius(100);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}


bool UTalkComponent::GetResponseByIndex(int TalkIndex, int ResponseIndex, FResponse &ResponseReturn)
{
	if (TalkSettings.IsValidIndex(TalkIndex))
	{
		if (TalkSettings[TalkIndex].Responses.IsValidIndex(ResponseIndex))
		{
			ResponseReturn = TalkSettings[TalkIndex].Responses[ResponseIndex];
			return true;
		}
	}

	return false;
}

void UTalkComponent::ActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* _CharacterReference = Cast<APlayerCharacter>(OtherActor))
	{
		UTalkWidget* _TalkWidgetReference = _CharacterReference->TalkWidget;
		if (_TalkWidgetReference != nullptr)
		{
			if (_TalkWidgetReference->GetRootResponsesWidget()->GetVisibility() == ESlateVisibility::Visible)
			{
				_TalkWidgetReference->UpdateTalk(-1, CharacterController);
			}
		}
	}
}

void UTalkComponent::TalkEnded()
{
	UTalkWidget* _TalkWidgetReference = CharacterController->GetPlayerCharacter()->TalkWidget;
	_TalkWidgetReference->RemoveFromViewport();
}

// Called when the game starts
void UTalkComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	this->OnComponentEndOverlap.AddDynamic(this, &UTalkComponent::ActorEndOverlap);
}

FText UTalkComponent::GetRandomFarewellText()
{
	if (GetFarewellMessages().Num() > 1)
	{
		int32 _FarewellAmount = GetFarewellMessages().Num();
		return FarewellMessages[FMath::RandRange(0, (_FarewellAmount - 1))];
	}
	else if (GetFarewellMessages().Num() > 0)
	{
		return FarewellMessages[0];
	}
	return FText::FromString("");
}

FText UTalkComponent::GetNPCName()
{
	UCharacterIdentity* _IdentityReference = GetOwner()->FindComponentByClass<UCharacterIdentity>();
	if (_IdentityReference != nullptr)
	{
		return _IdentityReference->GetCharacterName();
	}
	return FText();
}

void UTalkComponent::StartTalk(ACharacterController* ControllerReference)
{
	CharacterController = ControllerReference;
	GetCharacterController()->UpdateIdentifiersVisibility(false);
	GetCharacterController()->GetPlayerCharacter()->GetActionComponent()->Busy();
	UTalkWidget* _TalkWidgetReference = GetCharacterController()->GetPlayerCharacter()->TalkWidget;
	GetCharacterController()->AddWidgetEnablingMouse(_TalkWidgetReference);
	_TalkWidgetReference->SetTalkComponent(this);
	_TalkWidgetReference->UpdateTalk(0, GetCharacterController());
	_TalkWidgetReference->AddToViewport(2);
	_TalkWidgetReference->ShowTalkAnimation();
	_TalkWidgetReference->SwitchResponsesVisibility();
	GetWorld()->GetTimerManager().ClearTimer(EndTalkHandle);
	float _CharacterDistance = FVector::Dist2D(GetOwner()->GetActorLocation(), GetCharacterController()->GetPlayerCharacter()->GetActorLocation());
	SetSphereRadius(_CharacterDistance);
}

void UTalkComponent::EndTalk()
{
	UTalkWidget* _TalkWidgetReference = CharacterController->GetPlayerCharacter()->TalkWidget;
	FText _FarewellText = _TalkWidgetReference->CurrentText;
	FString _ReducedText = _FarewellText.ToString().Replace(TEXT(" "), TEXT(""));
	float _TextSpeed = _ReducedText.Len() * 0.25f;
	GetCharacterController()->GetPlayerCharacter()->GetActionComponent()->Ready();
	GetCharacterController()->UpdateIdentifiersVisibility(true);
	GetCharacterController()->RemoveWidgetEnablingMouse(_TalkWidgetReference);
	_TalkWidgetReference->SwitchResponsesVisibility();
	GetWorld()->GetTimerManager().SetTimer(EndTalkHandle, this, &UTalkComponent::TalkEnded, _TextSpeed, false);
}

void UTalkComponent::ResponseClicked(int TalkIndex, int ResponseIndex, int ReturnTalkID)
{
	ResponseDelegate.Broadcast(TalkIndex, ResponseIndex, ReturnTalkID);
}


FTalk UTalkComponent::GetTalkByIndex(int Index)
{
	return TalkSettings[Index];
}
