// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemObject.generated.h"

class UStaticMeshComponent;
class UInteractableComponent;
class UHandComponent;
class UTexture2D;
class UItemObjectWidget;
class UPrimitiveComponent;

UCLASS()
class FALLOFKINGDOMS_API AItemObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemObject();

	// PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* ObjectMesh;
		UStaticMeshComponent* GetObjetMesh() const { return ObjectMesh; }

	UPROPERTY(EditAnywhere, Category = "Components")
		UInteractableComponent* InteractableComponent;
		UInteractableComponent* GetInteractableComponent() const { return InteractableComponent; }

	UPROPERTY(EditAnywhere, Transient, Category = "Widget")
		UItemObjectWidget* ItemObjectWidget;
		UItemObjectWidget* GetItemObjectWidget() const { return ItemObjectWidget; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		UTexture2D* ItemObjectImage;
		UTexture2D* GetItemObjectImage() const { return ItemObjectImage; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UItemObjectWidget> ItemObjectWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float ItemObjectWeight;
		float GetItemObjectWeight() const { return ItemObjectWeight; }

	// METHODS
	UFUNCTION()
		void Interacted(UHandComponent* HandReference);
};