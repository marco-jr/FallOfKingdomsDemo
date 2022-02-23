// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USkeletalMeshComponent;
class USpringArmComponent;
class UInteractableCameraComponent;
class UArrowComponent;
class USphereComponent;
class UActionComponent;
class UInteractableComponent;
class UPrimitiveComponent;

UCLASS()
class FALLOFKINGDOMS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// COMPONENTS
	UPROPERTY(VisibleAnywhere)
		USphereComponent* CommunicationSphere;
		USphereComponent* GetCommunicationSphere() const { return CommunicationSphere; }

	UPROPERTY(EditAnywhere, Category = "Components")
		USpringArmComponent* SpringArmComponent;
		USpringArmComponent* GetSpringArmComponent() const { return SpringArmComponent; }

	UPROPERTY(EditAnywhere, Category = "Components")
		UInteractableCameraComponent* CameraComponent;
		UInteractableCameraComponent* GetCameraComponent() const { return CameraComponent; }

	UPROPERTY(EditAnywhere, Category = "Components")
		UArrowComponent* ForwardArrow;
		UArrowComponent* GetForwardArrow() const { return ForwardArrow; }

	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* TorsoMeshComponent;
		USkeletalMeshComponent* GetTorsoMesh() const { return TorsoMeshComponent; }

	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* HandsMeshComponent;
		USkeletalMeshComponent* GetHandsMesh() const { return HandsMeshComponent; }

	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* LegsMeshComponent;
		USkeletalMeshComponent* GetLegsMesh() const { return LegsMeshComponent; }

	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* FeetMeshComponent;
		USkeletalMeshComponent* GetFeetMesh() const { return FeetMeshComponent; }

	UPROPERTY(EditAnywhere, Category = "Components")
		UActionComponent* ActionComponent;
		UActionComponent* GetActionComponent() const { return ActionComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// PROPERTIES
	UPROPERTY(VisibleAnywhere)
		UInteractableComponent* CurrentInteractable;
		UInteractableComponent* GetCurrentInteractable() const { return CurrentInteractable; }

	UPROPERTY(VisibleAnywhere)
		TArray<UInteractableComponent*> ClosestInteractables;
		TArray<UInteractableComponent*> GetClosestInteractables() const { return ClosestInteractables; }

	FTimerHandle InteractableTimer;

	// METHODS
	virtual void OnConstruction(const FTransform& Transform) override;

	void LookUp(float Value);
	void LookRight(float Value);
	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION()
		void CommunicationBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void AddInteractable(UInteractableComponent* InteractableReference);
	
	UFUNCTION()
		void RemoveInteractable(UInteractableComponent* InteractableReference);
	
	UFUNCTION()
		void UpdateInteractable();

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
