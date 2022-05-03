// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugActor.h"
#include "TalkComponent.h"
#include "InteractableComponent.h"
#include "HandComponent.h"
#include "PlayerCharacter.h"
#include "Components/SceneComponent.h"
#include "CharacterIdentity.h"
#include "Components/SphereComponent.h"
#include "HealthComponent.h"

// Sets default values
ADebugActor::ADebugActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractableComponent"));
	TalkComponent = CreateDefaultSubobject<UTalkComponent>(TEXT("TalkComponent"));
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CharacterIdentity = CreateDefaultSubobject<UCharacterIdentity>(TEXT("CharacterIdentity"));
	TestSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Debug Sphere"));

	RootComponent = SceneComponent;

	InteractableComponent->SetupAttachment(GetRootComponent());
	TalkComponent->SetupAttachment(GetRootComponent());
	CharacterIdentity->SetupAttachment(GetRootComponent());
	TestSphere->SetupAttachment(GetRootComponent());

	TestSphere->InitSphereRadius(200);
}

// Called when the game starts or when spawned
void ADebugActor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableComponent->InteractDelegate.AddDynamic(this, &ADebugActor::StartTalk);
	TestSphere->OnComponentBeginOverlap.AddDynamic(this, &ADebugActor::DebugSphereOverlap);
}

void ADebugActor::StartTalk(UHandComponent* HandReference)
{
	APlayerCharacter* _CharacterReference = Cast<APlayerCharacter>(HandReference->GetOwner());

	if (_CharacterReference != nullptr)
	{
		TalkComponent->StartTalk(_CharacterReference->ControllerReference);
	}
}

void ADebugActor::DebugSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* _CharacterReference = Cast<APlayerCharacter>(OtherActor))
	{
		_CharacterReference->GetHealthComponent()->ChangeHealthByValue(EHealthType::Calories, 150);
	}
}
