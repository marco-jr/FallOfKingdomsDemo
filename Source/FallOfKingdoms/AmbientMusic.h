// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmbientMusic.generated.h"

class USceneComponent;
class USphereComponent;
class APlayerCharacter;

UCLASS()
class FALLOFKINGDOMS_API AAmbientMusic : public AActor
{
	GENERATED_BODY()
	

public:
	AAmbientMusic();

	// SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		TArray<USoundBase*> MusicSounds;
		TArray<USoundBase*> GetMusicSounds() const { return MusicSounds; }

	// COMPONENTS
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* SceneComponent;
		USceneComponent* GetSceneComponent() const { return SceneComponent; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USphereComponent* CommunicationSphere;
		USphereComponent* GetCommunicationSphere() const { return CommunicationSphere; } 

	// FUNCTIONS
	UFUNCTION()
		USoundBase* GetRandomMusicSound();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bDynamicMusicVolume;
		bool GetDynamicMusicVolume() const { return bDynamicMusicVolume; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float MinimalVolumeMultiplier;
		float GetMinimalVolumeMultiplier() const { return MinimalVolumeMultiplier; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float SphereRadius;
		float GetSphereRadius() const { return SphereRadius; }

	UPROPERTY(VisibleAnywhere, Category = "Logic")
		APlayerCharacter* CharacterReference;
		APlayerCharacter* GetCharacterReference() const { return CharacterReference; }

	FTimerHandle MusicUpdateHandle;


	// METHODS
	UFUNCTION()
		void CommunicationBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void CommunicationEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void StartMusic(APlayerCharacter* NewCharacter);

	UFUNCTION()
		void StopMusic();

	UFUNCTION()
		void UpdateMusicVolume();
};
