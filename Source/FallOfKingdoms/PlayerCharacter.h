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
class UActionMessageWidget;
class UInteractableComponent;
class UInteractableWidget;
class ACharacterController;
class UHandComponent;
class UPrimitiveComponent;
class UTalkWidget;
class UWidgetComponent;
class UIdentifierWidget;
class UCharacterHUD;
class UHealthComponent;
class UAudioComponent;
class AAmbientMusic;

UCLASS()
class FALLOFKINGDOMS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Settings")
		float MusicVolume;
		float GetMusicVolume() const { return MusicVolume; }

	UPROPERTY(VisibleAnywhere, Category = "Logic")
		UTalkWidget* TalkWidget;
		UTalkWidget* GetTalkWidget() const { return TalkWidget; }
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UAudioComponent* DefaultMusicAudioComponent;
		UAudioComponent* GetDefaultMusicAudioComponent() const { return DefaultMusicAudioComponent; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UAudioComponent* MusicAudioComponent;
		UAudioComponent* GetMusicAudioComponent() const { return MusicAudioComponent; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UHealthComponent* HealthComponent;
		UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* CommunicationSphere;
		USphereComponent* GetCommunicationSphere() const { return CommunicationSphere; }

	UPROPERTY(EditAnywhere, Category = "Components")
		UHandComponent* RightHand;
		UHandComponent* GetRightHand() const { return RightHand; }

	UPROPERTY(EditAnywhere, Category = "Components")
		UHandComponent* LeftHand;
		UHandComponent* GetLeftHand() const { return LeftHand; }

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

	UPROPERTY(BlueprintReadOnly, Transient)
		ACharacterController* ControllerReference;
		ACharacterController* GetControllerReference() const { return ControllerReference; }

	// FUNCTIONS
	UFUNCTION()
		USoundBase* GetRandomMusicSound();

	UFUNCTION()
		void PlayMusic(AAmbientMusic* AmbientMusicReference = nullptr, float StartVolume = 1);

	UFUNCTION()
		FVector GetFaceForward();

	UFUNCTION()
		void UpdateIdentifiersVisibility(bool bVisibility);

	UFUNCTION()
		void ShowMessage(FText Message, EMessageType MessageType);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// PROPERTIES
	UPROPERTY(VisibleAnywhere, Transient)
		AAmbientMusic* CurrentAmbientMusic;
		AAmbientMusic* GetCurrentAmbientMusic() const { return CurrentAmbientMusic; }

	UPROPERTY(VisibleAnywhere, Transient)
		UCharacterHUD* HUD;
		UCharacterHUD* GetHUD() const { return HUD; }

	UPROPERTY(VisibleAnywhere, Transient, Category = "Components")
		UWidgetComponent* IdentifierWidgetComponent;
		UWidgetComponent* GetIdentifierWidgetComponent() const { return IdentifierWidgetComponent; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UIdentifierWidget* IdentifierWidget;
		UIdentifierWidget* GetIdentifierWidget() const { return IdentifierWidget; }

	UPROPERTY(VisibleAnywhere, Transient, Category = "Components")
		UWidgetComponent* InteractableWidgetComponent;
		UWidgetComponent* GetInteractableWidgetComponent() const { return InteractableWidgetComponent; }

	UPROPERTY(VisibleAnywhere, Transient, Category = "Components")
		UInteractableWidget* InteractableWidget;
		UInteractableWidget* GetInteractableWidget() const { return InteractableWidget; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		UCurveFloat* FOVCurve;
		UCurveFloat* GetFOVCurve() const { return FOVCurve; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UCharacterHUD> HUDClass;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UIdentifierWidget> IdentifierWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UInteractableWidget> InteractableWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UTalkWidget> TalkWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TArray<USoundBase*> DefaultMusicSounds;
		TArray<USoundBase*> GetDefaultMusicSounds() const { return DefaultMusicSounds; }

	UPROPERTY(VisibleAnywhere, Transient)
		UInteractableComponent* VisionInteractable;
		UInteractableComponent* GetVisionInteractable() const { return VisionInteractable; }

	UPROPERTY(VisibleAnywhere, Transient)
		TArray<UInteractableComponent*> ClosestInteractables;
		TArray<UInteractableComponent*> GetClosestInteractables() const { return ClosestInteractables; }
	
	UPROPERTY(VisibleAnywhere, Transient)
		UCharacterIdentity* VisionIdentifier;
		UCharacterIdentity* GetVisionIdentifier() const { return VisionIdentifier; }

	UPROPERTY(VisibleAnywhere, Transient)
		TArray<UCharacterIdentity*> ClosestIdentifiers;
		TArray<UCharacterIdentity*> GetClosestIdentifiers() const { return ClosestIdentifiers; }

	FTimerHandle InteractableTimer;
	FTimerHandle IdentifierTimer;
	FTimerHandle SpringArmTimer;
	float RightHandClickTime;
	float LeftHandClickTime;

	// METHODS
	virtual void OnConstruction(const FTransform& Transform) override;

	void LookUp(float Value);
	void LookRight(float Value);
	void MoveForward(float Value);
	void MoveRight(float Value);

	void Interact();
	void UseRightHand();
	void UseLeftHand();
	void Drop();
	void SwapHands();
	void SwapMouseVisibility();

	UFUNCTION()
		void UpdateSpringArm();

	UFUNCTION()
		void Interacted(UActionComponent* ActionReference);

	UFUNCTION()
		void CommunicationBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void CommunicationEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		
	UFUNCTION()
		void AddInteractable(UInteractableComponent* InteractableReference);
	
	UFUNCTION()
		void RemoveInteractable(UInteractableComponent* InteractableReference);
	
	UFUNCTION()
		void UpdateInteractable();

	UFUNCTION()
		void AddIdentifier(UCharacterIdentity* IdentifierReference);

	UFUNCTION()
		void RemoveIdentifier(UCharacterIdentity* IdentifierReference);

	UFUNCTION()
		void UpdateIdentifiers();

	UFUNCTION()
		void SoundMusicFinished();

	UFUNCTION()
		void DefaultSoundMusicFinished();

	// DEBUG
		void Debug();

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};