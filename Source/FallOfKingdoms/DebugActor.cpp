// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugActor.h"
#include "TalkComponent.h"
#include "InteractableComponent.h"
#include "HandComponent.h"
#include "PlayerCharacter.h"
#include "Components/SceneComponent.h"
#include "CharacterIdentity.h"

// Sets default values
ADebugActor::ADebugActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	TalkComponent = CreateDefaultSubobject<UTalkComponent>(TEXT("TalkComponent"));
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CharacterIdentity = CreateDefaultSubobject<UCharacterIdentity>(TEXT("CharacterIdentity"));

	RootComponent = SceneComponent;

	InteractableComponent->SetupAttachment(GetRootComponent());
	TalkComponent->SetupAttachment(GetRootComponent());
	CharacterIdentity->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADebugActor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableComponent->InteractDelegate.AddDynamic(this, &ADebugActor::StartTalk);
}

void ADebugActor::StartTalk(UHandComponent* HandReference)
{
	APlayerCharacter* _CharacterReference = Cast<APlayerCharacter>(HandReference->GetOwner());

	if (_CharacterReference != nullptr)
	{
		TalkComponent->StartTalk(_CharacterReference->ControllerReference);
	}
}