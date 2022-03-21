// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugActor.generated.h"

class UInteractableComponent;
class UTalkComponent;
class UHandComponent;
class USceneComponent;
class UCharacterIdentity;

UCLASS()
class FALLOFKINGDOMS_API ADebugActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADebugActor();

	// PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Components")
		UCharacterIdentity* CharacterIdentity;
		UCharacterIdentity* GetCharacterIdentity() const { return CharacterIdentity; }

	UPROPERTY(EditAnywhere, Category = "Components")
		UInteractableComponent* InteractableComponent;
		UInteractableComponent* GetInteractableComponent() const { return InteractableComponent; }

	UPROPERTY(EditAnywhere, Category = "Components")
		UTalkComponent* TalkComponent;
		UTalkComponent* GetTalkComponent() const { return TalkComponent; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* SceneComponent;
		USceneComponent* GetSceneComponent() const { return SceneComponent; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// METHODS
	UFUNCTION()
	void StartTalk(UHandComponent* HandReference);
};
