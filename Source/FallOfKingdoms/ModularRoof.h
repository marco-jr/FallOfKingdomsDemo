// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModularRoof.generated.h"

class USplineComponent;
class UStaticMesh;
class USceneComponent;

UCLASS()
class FALLOFKINGDOMS_API AModularRoof : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModularRoof();

	// PROPERTIES
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* SceneRoot;
		USceneComponent* GetSceneRoot() const { return SceneRoot; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USplineComponent* RoofSpline;
		USplineComponent* GetRoofSpline() const { return RoofSpline; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		UStaticMesh* RoofMesh;
		UStaticMesh* GetRoofMesh() const { return RoofMesh; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		UStaticMesh* RoofEdgeMesh;
		UStaticMesh* GetRoofEdgeMesh() const { return RoofEdgeMesh; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
};
