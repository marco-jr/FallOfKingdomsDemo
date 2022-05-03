// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModularSpawner.generated.h"

class UHierarchicalInstancedStaticMeshComponent;
class UStaticMesh;

UCLASS()
class FALLOFKINGDOMS_API AModularSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModularSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// PROPERTIES
	UPROPERTY(VisibleAnywhere, Category = "Logic")
		TArray<UHierarchicalInstancedStaticMeshComponent*> CreatedInstances;
		TArray<UHierarchicalInstancedStaticMeshComponent*> GetCreatedInstances() const { return CreatedInstances; }

	// FUNCTIONS
	UFUNCTION()
		UHierarchicalInstancedStaticMeshComponent* GetCreatedInstanceByMesh(UStaticMesh* MeshReference);

	// METHODS
	UFUNCTION()
		void CreateWalls(TArray<AActor*> WallReferences);

	UFUNCTION()
		void CreateFloors(TArray<AActor*> FloorReferences);

	UFUNCTION()
		void CreateRoofs(TArray<AActor*> RoofReferences);
};
