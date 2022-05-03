// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InteractableCameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "ActionComponent.h"
#include "HandComponent.h"
#include "InteractableComponent.h"
#include "ActionMessageWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharacterController.h"
#include "FOKFunctionsLibrary.h"
#include "CharacterIdentity.h"
#include "Components/WidgetComponent.h"
#include "InteractableWidget.h"
#include "IdentifierWidget.h"
#include "CharacterHUD.h"
#include "HealthComponent.h"
#include "Components/AudioComponent.h"
#include "AmbientMusic.h"


// DEBUG INCLUDE
#include "TalkWidget.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TorsoMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso Mesh Component"));
	HandsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands Mesh Component"));
	LegsMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs Mesh Component"));
	FeetMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet Mesh Component"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	CameraComponent = CreateDefaultSubobject<UInteractableCameraComponent>(TEXT("Camera Component"));
	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Forward Arrow"));
	ActionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("Action Component"));
	CommunicationSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Communication Sphere"));
	RightHand = CreateDefaultSubobject<UHandComponent>(TEXT("Right Hand"));
	LeftHand = CreateDefaultSubobject<UHandComponent>(TEXT("Left Hand"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicAudioComponent"));
	DefaultMusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DefaultMusicAudioComponent"));
	InteractableWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interactable Widget Component"));
	IdentifierWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Identifier Widget Component"));

	TorsoMeshComponent->SetupAttachment(GetMesh());
	HandsMeshComponent->SetupAttachment(GetMesh());
	LegsMeshComponent->SetupAttachment(GetMesh());
	FeetMeshComponent->SetupAttachment(GetMesh());
	CameraComponent->SetupAttachment(GetSpringArmComponent());
	ForwardArrow->SetupAttachment(GetCameraComponent());
	MusicAudioComponent->SetupAttachment(GetCameraComponent());
	DefaultMusicAudioComponent->SetupAttachment(GetCameraComponent());
	CommunicationSphere->SetupAttachment(GetRootComponent());
	RightHand->SetupAttachment(GetMesh());
	LeftHand->SetupAttachment(GetMesh());
	SpringArmComponent->SetupAttachment(GetMesh());

	TorsoMeshComponent->MasterPoseComponent = GetMesh();
	HandsMeshComponent->MasterPoseComponent = GetMesh();
	LegsMeshComponent->MasterPoseComponent = GetMesh();
	FeetMeshComponent->MasterPoseComponent = GetMesh();

	CommunicationSphere->SetSphereRadius(100);
	CommunicationSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CommunicationSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CommunicationSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CommunicationSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::CommunicationBeginOverlap);
	CommunicationSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::CommunicationEndOverlap);

	InteractableWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	InteractableWidgetComponent->SetVisibility(false);

	IdentifierWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	IdentifierWidgetComponent->SetVisibility(false);

	MusicVolume = 0.1;
}

USoundBase* APlayerCharacter::GetRandomMusicSound()
{
	USoundBase* _RandomSound = GetDefaultMusicSounds()[FMath::RandRange(0, GetDefaultMusicSounds().Num() - 1)];
	return _RandomSound != nullptr ? _RandomSound : nullptr;
}

void APlayerCharacter::PlayMusic(AAmbientMusic* AmbientMusicReference, float StartVolume)
{
	CurrentAmbientMusic = AmbientMusicReference;

	if (GetCurrentAmbientMusic() != nullptr)
	{
		GetMusicAudioComponent()->SetSound(GetCurrentAmbientMusic()->GetRandomMusicSound());
		GetDefaultMusicAudioComponent()->SetVolumeMultiplier((1 - StartVolume) * MusicVolume);
		GetMusicAudioComponent()->SetVolumeMultiplier(StartVolume);
		GetMusicAudioComponent()->Play();
	}
	else
	{
		GetMusicAudioComponent()->Stop();
		GetDefaultMusicAudioComponent()->SetVolumeMultiplier(StartVolume * MusicVolume);
		if (GetDefaultMusicAudioComponent()->Sound == nullptr)
		{
			GetDefaultMusicAudioComponent()->SetSound(GetRandomMusicSound());
			GetDefaultMusicAudioComponent()->Play();
		}
	}	
}

FVector APlayerCharacter::GetFaceForward()
{
	FRotator _ForwardRotator = FRotator(0, GetForwardArrow()->GetComponentRotation().Yaw, GetForwardArrow()->GetComponentRotation().Roll);
	
	return _ForwardRotator.Vector();
}

void APlayerCharacter::UpdateIdentifiersVisibility(bool bVisibility)
{
	if (VisionInteractable != nullptr)
	{
		InteractableWidgetComponent->SetVisibility(bVisibility);
	}

	if (VisionIdentifier != nullptr)
	{
		IdentifierWidgetComponent->SetVisibility(bVisibility);
	}
}

void APlayerCharacter::ShowMessage(FText Message, EMessageType MessageType)
{
	GetHUD()->ActionMessage->ShowMessage(Message, MessageType);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpringArmTimer, this, &APlayerCharacter::UpdateSpringArm, 0.01f, true);

	GetMusicAudioComponent()->OnAudioFinished.AddDynamic(this, &APlayerCharacter::SoundMusicFinished);
	GetDefaultMusicAudioComponent()->OnAudioFinished.AddDynamic(this, &APlayerCharacter::DefaultSoundMusicFinished);

	ControllerReference = Cast<ACharacterController>(GetController());
		
	InteractableWidget = CreateWidget<UInteractableWidget>(ControllerReference, InteractableWidgetClass);
	GetInteractableWidgetComponent()->SetWidget(InteractableWidget);

	IdentifierWidget = CreateWidget<UIdentifierWidget>(ControllerReference, IdentifierWidgetClass);
	GetIdentifierWidgetComponent()->SetWidget(IdentifierWidget);

	TalkWidget = CreateWidget<UTalkWidget>(ControllerReference, TalkWidgetClass);
	
	HUD = CreateWidget<UCharacterHUD>(ControllerReference, HUDClass);
	HUD->AddToViewport(1);

	TSet<UPrimitiveComponent*> _OverlappedComponents;
	GetMesh()->GetOverlappingComponents(_OverlappedComponents);

	for (UPrimitiveComponent* _CurrentComponent : _OverlappedComponents)
	{
		if (UInteractableComponent* _InteractableComponent = Cast<UInteractableComponent>(_CurrentComponent))
		{
			ClosestInteractables.AddUnique(_InteractableComponent);
		}
		else if (UCharacterIdentity* _IdentifierComponent = Cast<UCharacterIdentity>(_CurrentComponent))
		{
			ClosestIdentifiers.AddUnique(_IdentifierComponent);
		}
	}

	TArray<AActor*> _OverlappedActors;
	GetMesh()->GetOverlappingActors(_OverlappedActors);

	for (AActor* _CurrentActor : _OverlappedActors)
	{
		if (AAmbientMusic* _AmbientMusicReference = Cast<AAmbientMusic>(_CurrentActor))
		{
			PlayMusic(_AmbientMusicReference);
		}
	}

	if (GetCurrentAmbientMusic() == nullptr)
	{
		PlayMusic();
	}

	UpdateInteractable();
	UpdateIdentifiers();
}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	RightHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("RightHandSocket"));
	LeftHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("LeftHandSocket"));
	SpringArmComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("HeadSocket"));
}

void APlayerCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void APlayerCharacter::LookRight(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter::MoveForward(float Value)
{
	FRotator _ForwardRotator = FRotator(0, GetForwardArrow()->GetComponentRotation().Yaw, GetForwardArrow()->GetComponentRotation().Roll);
	AddMovementInput(_ForwardRotator.Vector(), Value);
}

void APlayerCharacter::MoveRight(float Value)
{
	FRotator _RightRotator = FRotator(0, GetForwardArrow()->GetComponentRotation().Yaw + 90, GetForwardArrow()->GetComponentRotation().Roll);
	AddMovementInput(_RightRotator.Vector(), Value);
}

void APlayerCharacter::Interact()
{
	if (VisionInteractable != nullptr)
	{
		float _RotationYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), VisionInteractable->GetComponentLocation()).Yaw;

		if (VisionInteractable->GetHandFreeNeeded() && UFOKFunctionsLibrary::GetFreeHand(this) || !VisionInteractable->GetHandFreeNeeded())
		{
			if (VisionInteractable->GetAnimateInteraction())
			{
				if (GetActionComponent()->GetCanDoActions(true))
				{
					SetActorRotation(FRotator(0, _RotationYaw, 0));
					ActionComponent->DoAction(ECurrentAction::Interact, 0);
					ActionComponent->ActionCompletedDelegate.AddDynamic(this, &APlayerCharacter::Interacted);
				}
			}
			else
			{
					
				SetActorRotation(FRotator(0, _RotationYaw, 0));
				Interacted(GetActionComponent());
			}
		}
		else
		{
			ControllerReference->ShowMessage(FText::FromString("Vou precisar de uma mão livre."), EMessageType::Warning);
		}
	}
}

void APlayerCharacter::Interacted(UActionComponent* ActionReference)
{
	UHandComponent* _InteractHand;
	if (IsValid(UFOKFunctionsLibrary::GetFreeHand(this)))
	{
		_InteractHand = UFOKFunctionsLibrary::GetFreeHand(this);
	}
	else
	{
		_InteractHand = RightHand;
	}
	VisionInteractable->Interacted(_InteractHand);
}

void APlayerCharacter::CommunicationBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UInteractableComponent* _Interactable = Cast<UInteractableComponent>(OtherComp))
	{
		AddInteractable(_Interactable);
	}

	if (UCharacterIdentity* _Identifier = Cast<UCharacterIdentity>(OtherComp))
	{
		AddIdentifier(_Identifier);
	}
}

void APlayerCharacter::CommunicationEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (UInteractableComponent* _Interactable = Cast<UInteractableComponent>(OtherComp))
	{
		RemoveInteractable(_Interactable);
	}

	if (UCharacterIdentity* _Identifier = Cast<UCharacterIdentity>(OtherComp))
	{
		RemoveIdentifier(_Identifier);
		UE_LOG(LogTemp, Warning, TEXT("EndIdentifier"));
	}
}

void APlayerCharacter::AddIdentifier(UCharacterIdentity* IdentifierReference)
{
	ClosestIdentifiers.AddUnique(IdentifierReference);

	if (ClosestIdentifiers.Num() == 1)
	{
		GetWorld()->GetTimerManager().SetTimer(IdentifierTimer, this, &APlayerCharacter::UpdateIdentifiers, 0.25f, true);
	}
}

void APlayerCharacter::RemoveIdentifier(UCharacterIdentity* IdentifierReference)
{
	if (ClosestIdentifiers.Contains(IdentifierReference))
	{
		if (ClosestIdentifiers.Num() == 1)
		{
			GetWorld()->GetTimerManager().ClearTimer(IdentifierTimer);
		}

		if (VisionIdentifier == IdentifierReference)
		{
			IdentifierWidgetComponent->SetVisibility(false);
			VisionIdentifier = nullptr;
			if (GetActionComponent()->GetCurrentAction() == ECurrentAction::Interact)
			{
				GetActionComponent()->Ready();
			}
		}
		ClosestIdentifiers.Remove(IdentifierReference);
	}
}

void APlayerCharacter::UpdateIdentifiers()
{
	FVector _TraceStart = GetMesh()->GetSocketLocation(FName("HeadSocket"));
	FVector _TraceEnd = GetActorLocation() + (GetForwardArrow()->GetForwardVector() * 1000);
	FCollisionQueryParams _TraceQuery;
	FHitResult _HitResult;
	GetWorld()->LineTraceSingleByChannel(_HitResult, _TraceStart, _TraceEnd, ECC_Visibility, _TraceQuery);
	FVector _LocationReference = _HitResult.bBlockingHit ? _HitResult.Location : _HitResult.TraceEnd;
	if (ClosestIdentifiers.Num() > 0)
	{
		for (UCharacterIdentity* _CurrentIdentifier : ClosestIdentifiers)
		{
			FVector _IdentifierBoxExtend;
			_CurrentIdentifier->GetOwner()->GetActorBounds(true, _TraceEnd, _IdentifierBoxExtend);
			GetWorld()->LineTraceSingleByChannel(_HitResult, _TraceStart, _TraceEnd, ECC_Visibility, _TraceQuery);
			if (_HitResult.bBlockingHit == false || _HitResult.Actor == _CurrentIdentifier->GetOwner())
			{
				if (VisionIdentifier == nullptr)
				{
					VisionIdentifier = _CurrentIdentifier;
					IdentifierWidget->SetActorName(VisionIdentifier->GetCharacterName());
					IdentifierWidgetComponent->AttachToComponent(VisionIdentifier, FAttachmentTransformRules::SnapToTargetIncludingScale);
					IdentifierWidgetComponent->SetVisibility(true);
				}
				else if (FVector::Dist2D(_TraceEnd, _LocationReference) < FVector::Dist2D(VisionIdentifier->GetOwner()->GetActorLocation(), _LocationReference))
				{
					VisionIdentifier = _CurrentIdentifier;
					IdentifierWidget->SetActorName(VisionIdentifier->GetCharacterName());
					IdentifierWidgetComponent->AttachToComponent(VisionIdentifier, FAttachmentTransformRules::SnapToTargetIncludingScale);
				}
			}
			else if (_CurrentIdentifier == VisionIdentifier)
			{
				IdentifierWidgetComponent->SetVisibility(false);
				VisionIdentifier = nullptr;
			}
		}
	}
}

void APlayerCharacter::SoundMusicFinished()
{
	if (GetCurrentAmbientMusic() != nullptr)
	{
		GetMusicAudioComponent()->SetSound(GetCurrentAmbientMusic()->GetRandomMusicSound());
		GetMusicAudioComponent()->Play();
	}
}

void APlayerCharacter::DefaultSoundMusicFinished()
{
	GetDefaultMusicAudioComponent()->SetSound(GetRandomMusicSound());
	GetDefaultMusicAudioComponent()->Play();
}

void APlayerCharacter::AddInteractable(UInteractableComponent* InteractableReference)
{
	ClosestInteractables.AddUnique(InteractableReference);

	if (ClosestInteractables.Num() == 1)
	{
		GetWorld()->GetTimerManager().SetTimer(InteractableTimer, this, &APlayerCharacter::UpdateInteractable, 0.1f, true);
	}
}

void APlayerCharacter::RemoveInteractable(UInteractableComponent* InteractableReference)
{
	if(ClosestInteractables.Contains(InteractableReference))
	{
		ClosestInteractables.Remove(InteractableReference);

		if (ClosestInteractables.Num() == 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(InteractableTimer);
		}

		if (VisionInteractable == InteractableReference)
		{
			InteractableWidgetComponent->SetVisibility(false);
			VisionInteractable = nullptr;
			if (GetActionComponent()->GetCurrentAction() == ECurrentAction::Interact)
			{
				GetActionComponent()->Ready();
			}
		}

	}
}

void APlayerCharacter::UpdateInteractable()
{
	FVector _TraceStart = GetMesh()->GetSocketLocation(FName("HeadSocket"));
	FVector _TraceEnd;
	FCollisionQueryParams _TraceQuery;
	FHitResult _HitResult;
	FVector _LocationReference = GetActorLocation() + (GetForwardArrow()->GetForwardVector() * 100);

	if (ClosestInteractables.Num() > 0)
	{
		for (UInteractableComponent* _CurrentInteractable : ClosestInteractables)
		{
			_TraceEnd = _CurrentInteractable->GetComponentLocation();
			GetWorld()->LineTraceSingleByChannel(_HitResult, _TraceStart, _TraceEnd, ECC_Visibility, _TraceQuery);
			if (_HitResult.bBlockingHit == false || _HitResult.Actor == _CurrentInteractable->GetOwner())
			{
				if (VisionInteractable == nullptr)
				{
					VisionInteractable = _CurrentInteractable;
					InteractableWidget->SetActionText(VisionInteractable->InteractionText);
					InteractableWidget->SetActionImage(VisionInteractable->InteractionImage);
					InteractableWidgetComponent->AttachToComponent(VisionInteractable, FAttachmentTransformRules::SnapToTargetIncludingScale);
					InteractableWidgetComponent->SetVisibility(true);
				}
				else if (FVector::Dist2D(_CurrentInteractable->GetComponentLocation(), _LocationReference) < FVector::Dist2D(VisionInteractable->GetComponentLocation(), _LocationReference))
				{
					VisionInteractable = _CurrentInteractable;
					InteractableWidget->SetActionText(VisionInteractable->InteractionText);
					InteractableWidget->SetActionImage(VisionInteractable->InteractionImage);
					InteractableWidgetComponent->AttachToComponent(VisionInteractable, FAttachmentTransformRules::SnapToTargetIncludingScale);
				}
			}
			else if (_CurrentInteractable == VisionInteractable)
			{
				InteractableWidgetComponent->SetVisibility(false);
				VisionInteractable = nullptr;
			}
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// AXIS INPUTS
	InputComponent->BindAxis("MouseUp", this, &APlayerCharacter::LookUp);
	InputComponent->BindAxis("MouseRight", this, &APlayerCharacter::LookRight);
	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// ACTIONS INPUT
	InputComponent->BindAction("Attack", IE_Pressed, ActionComponent, &UActionComponent::Attack);
	InputComponent->BindAction("Sprint", IE_Pressed, ActionComponent, &UActionComponent::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, ActionComponent, &UActionComponent::StopSprint);
	InputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	InputComponent->BindAction("UseRightHand", IE_Pressed, this, &APlayerCharacter::UseRightHand);
	InputComponent->BindAction("UseLeftHand", IE_Pressed, this, &APlayerCharacter::UseLeftHand);
	InputComponent->BindAction("Drop", IE_Pressed, this, &APlayerCharacter::Drop);
	InputComponent->BindAction("SwapHands", IE_Pressed, this, &APlayerCharacter::SwapHands);
	InputComponent->BindAction("Debug", IE_Pressed, this, &APlayerCharacter::Debug);
	InputComponent->BindAction("FreeMouse", IE_Pressed, this, &APlayerCharacter::SwapMouseVisibility);
}

void APlayerCharacter::UseRightHand()
{
	RightHand->UseItemObject();
}

void APlayerCharacter::Drop()
{
	RightHand->RemoveItemObject(true);
}

void APlayerCharacter::UseLeftHand()
{
	LeftHand->UseItemObject();
}

void APlayerCharacter::SwapHands()
{
	AItemObject* _RightHoldItemObject = GetRightHand()->GetItemObject();
	AItemObject* _LeftHoldItemObject = GetLeftHand()->GetItemObject();
	GetRightHand()->RemoveItemObject(false);
	GetLeftHand()->RemoveItemObject(false);
	GetRightHand()->AddItemObject(_LeftHoldItemObject);
	GetLeftHand()->AddItemObject(_RightHoldItemObject);
}

void APlayerCharacter::SwapMouseVisibility()
{
	ControllerReference->UpdateMouseCursor(!ControllerReference->bShowMouseCursor);
}

void APlayerCharacter::UpdateSpringArm()
{
	float _ArmLength = GetSpringArmComponent()->TargetArmLength;
	float _BaseFieldOfView = 70;
	float _CurrentDistance = FVector::Dist(GetSpringArmComponent()->GetComponentLocation(), GetSpringArmComponent()->GetSocketTransform(FName()).GetLocation());
	float _FieldOfView = _BaseFieldOfView;
	if (GetFOVCurve()->IsValidLowLevel())
	{
		float _CurrentValue = (1 - _CurrentDistance / _ArmLength);
		_FieldOfView = _BaseFieldOfView + GetFOVCurve()->GetFloatValue(_CurrentValue);
	}
	else
	{
		_FieldOfView = _BaseFieldOfView + (1 - _CurrentDistance / _ArmLength) * 40;
	}
	UE_LOG(LogTemp, Warning, TEXT("%f"), _ArmLength);
	GetCameraComponent()->SetFieldOfView(_FieldOfView);
}

void APlayerCharacter::Debug()
{
	HUD->ActionMessage->ShowMessage(FText::FromString("Debug Teste."), EMessageType::Default);
}