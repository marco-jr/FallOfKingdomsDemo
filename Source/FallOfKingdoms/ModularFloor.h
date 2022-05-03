// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModularFloor.generated.h"

class UStaticMesh;
class USceneComponent;
class USplineComponent;
class UInstancedStaticMeshComponent;

UCLASS()
class FALLOFKINGDOMS_API AModularFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModularFloor();

	// PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Settings")
		UStaticMesh* FloorMesh;
		UStaticMesh* GetFloorMesh() const { return FloorMesh; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* RootScene;
		USceneComponent* GetRootScene() const { return RootScene; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USplineComponent* FloorSpline;
		USplineComponent* GetFloorSpline() const { return FloorSpline; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
