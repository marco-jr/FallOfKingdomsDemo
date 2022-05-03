// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModularWall.generated.h"

class USplineComponent;
class UStaticMesh;
class USceneComponent;
class UMaterialInterface;
class UInstancedStaticMeshComponent;

USTRUCT(BlueprintType)
struct FSplineWallSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		UStaticMesh* WallMesh;

	UPROPERTY(EditAnywhere)
		bool bAddPillar;

	UPROPERTY(EditAnywhere)
		UStaticMesh* PillarMesh;
};

UCLASS()
class FALLOFKINGDOMS_API AModularWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModularWall();

	// PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Settings")
		TArray<FSplineWallSettings> WallsSettings;
		TArray<FSplineWallSettings> GetWallsSettings() const { return WallsSettings; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		FSplineWallSettings DefaultWallSetting;
		FSplineWallSettings GetDefaultWallSetting() const { return DefaultWallSetting; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* ActorRoot;
		USceneComponent* GetActorRoot() const { return ActorRoot; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USplineComponent* WallsSpline;
		USplineComponent* GetWallsSpline() const { return WallsSpline; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// FUNCTIONS
	UFUNCTION()
		UInstancedStaticMeshComponent* GetInstanceByMesh(UStaticMesh* MeshReference, TArray<UInstancedStaticMeshComponent*> InstancedWalls, TArray<UInstancedStaticMeshComponent*> InstancedPillars);
};
