// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InteractableCameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"
#include "ActionComponent.h"
#include "InteractableComponent.h"

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

	TorsoMeshComponent->SetupAttachment(GetMesh());
	HandsMeshComponent->SetupAttachment(GetMesh());
	LegsMeshComponent->SetupAttachment(GetMesh());
	FeetMeshComponent->SetupAttachment(GetMesh());
	SpringArmComponent->SetupAttachment(GetMesh(), FName("HeadSocket"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	ForwardArrow->SetupAttachment(CameraComponent);
	CommunicationSphere->SetupAttachment(RootComponent);

	TorsoMeshComponent->MasterPoseComponent = GetMesh();
	HandsMeshComponent->MasterPoseComponent = GetMesh();
	LegsMeshComponent->MasterPoseComponent = GetMesh();
	FeetMeshComponent->MasterPoseComponent = GetMesh();

	CommunicationSphere->SetSphereRadius(90);
	CommunicationSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CommunicationSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CommunicationSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CommunicationSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::CommunicationBeginOverlap);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

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

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	
}

void APlayerCharacter::CommunicationBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TArray<USceneComponent*> _OverlappedChildren;
	OverlappedComponent->GetChildrenComponents(false, _OverlappedChildren);
	for (USceneComponent* _Current : _OverlappedChildren)
	{
		if (UInteractableComponent* _CurrentInteractable = Cast<UInteractableComponent>(_Current))
		{
			AddInteractable(_CurrentInteractable);
		}
	}
}

void APlayerCharacter::AddInteractable(UInteractableComponent* InteractableReference)
{
	ClosestInteractables.AddUnique(InteractableReference);

	if (ClosestInteractables.Num() == 1)
	{
		GetWorld()->GetTimerManager().SetTimer(InteractableTimer, this, &APlayerCharacter::UpdateInteractable, 0.05f, true);
	}
}

void APlayerCharacter::RemoveInteractable(UInteractableComponent* InteractableReference)
{
	if(ClosestInteractables.Contains(InteractableReference))
	{
		ClosestInteractables.Remove(InteractableReference);
	}
}

void APlayerCharacter::UpdateInteractable()
{
	FVector _TraceStart = GetMesh()->GetSocketLocation(FName("HeadSocket"));
	FVector _TraceEnd;
	FCollisionQueryParams _TraceQuery;
	FHitResult _HitResult;

	if (ClosestInteractables.Num() > 0)
	{
		for (UInteractableComponent* _CurrentInteractable : ClosestInteractables)
		{
			_TraceEnd = _CurrentInteractable->GetComponentLocation();
			GetWorld()->LineTraceSingleByChannel(_HitResult, _TraceStart, _TraceEnd, ECC_Visibility, _TraceQuery);
			if (_HitResult.bBlockingHit == false || _HitResult.Actor == _CurrentInteractable->GetOwner())
			{
				if (CurrentInteractable != nullptr)
				{
					CurrentInteractable = _CurrentInteractable;
					CurrentInteractable->UpdateVisibility(true);
				}
				else if (FVector::Dist2D(_CurrentInteractable->GetComponentLocation(), GetActorLocation()) < FVector::Dist2D(CurrentInteractable->GetComponentLocation(), GetActorLocation()))
				{
					CurrentInteractable->UpdateVisibility(false);
					CurrentInteractable = _CurrentInteractable;
					CurrentInteractable->UpdateVisibility(true);
				}
			}
			else if (_CurrentInteractable == CurrentInteractable)
			{
				CurrentInteractable->UpdateVisibility(false);
				CurrentInteractable = nullptr;
			}
		}
	}
	else if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->UpdateVisibility(false);
		GetWorld()->GetTimerManager().ClearTimer(InteractableTimer);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractableTimer);
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
	InputComponent->BindAction("Sprint", IE_Pressed, ActionComponent, &UActionComponent::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, ActionComponent, &UActionComponent::StopSprint);
}

