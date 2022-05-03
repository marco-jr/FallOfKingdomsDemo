// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class ULockComponent;
class UStaticMeshComponent;
class UStaticMesh;
class USceneComponent;
class UInteractableComponent;
class UHandComponent;
class USoundBase;

UCLASS()
class FALLOFKINGDOMS_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	// PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Components")
		UInteractableComponent* InteractableComponent;
		UInteractableComponent* GetInteractableComponent() const { return InteractableComponent; }

	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* DoorStaticMesh;
		UStaticMeshComponent* GetDoorStaticMesh() const { return DoorStaticMesh; }

	UPROPERTY(EditAnywhere, Category = "Components")
		USceneComponent* SceneComponent;
		USceneComponent* GetSceneComponent() const { return SceneComponent; }

	UPROPERTY(EditAnywhere, Category = "Components")
		ULockComponent* LockComponent;
		ULockComponent* GetLockComponent() const { return LockComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		USoundBase* LockedDoorSound;
		USoundBase* GetLockedDoorSound() const { return LockedDoorSound; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		USoundBase* OpenDoorSound;
		USoundBase* GetOpenDoorSound() const { return OpenDoorSound; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		USoundBase* CloseDoorSound;
		USoundBase* GetCloseDoorSound() const { return CloseDoorSound; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		FText LockedMessage;
		FText GetLockedMessage() const { return LockedMessage; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float DoorRotateAmount;
		float GetDoorRotateAmount() const { return DoorRotateAmount; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		UStaticMesh* DoorMesh;
		UStaticMesh* GetDoorMesh() const { return DoorMesh; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bInverseDoor;
		bool GetIsInverseDoor() const { return bInverseDoor; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bClosing;
		bool GetIsClosing() const { return bClosing; }

	FTimerHandle DoorRotateHandle;

	// METHODS
	UFUNCTION()
		void Interacted(UHandComponent* HandReference);

	UFUNCTION()
		void DoorRotate();
};
