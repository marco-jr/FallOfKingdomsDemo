// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularWall.h"
#include "Components/SplineComponent.h"
#include "Components/SceneComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
AModularWall::AModularWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WallsSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Walls Spline"));
	ActorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Actor Root"));

	RootComponent = ActorRoot;
	WallsSpline->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AModularWall::BeginPlay()
{
	Super::BeginPlay();
	
}

void AModularWall::OnConstruction(const FTransform& Transform)
{
	TArray<UInstancedStaticMeshComponent*> _CreatedWalls;
	TArray<UInstancedStaticMeshComponent*> _CreatedPillars;
	int _WallSections = GetWallsSpline()->GetNumberOfSplinePoints() - 1;

	// RESIZE THE WALLS SETTINGS ARRAY
	if (GetWallsSettings().Num() < _WallSections)
	{
		for (int _CurrentIndex = 0; _CurrentIndex < _WallSections; _CurrentIndex++)
		{
			WallsSettings.Add(GetDefaultWallSetting());
		}
	}
	else if (GetWallsSettings().Num() > _WallSections)
	{
		WallsSettings.SetNum(_WallSections);
	}

	// GENERATE WALLS
	if (_WallSections > 0)
	{
		for (int _CurrentIndex = 0; _CurrentIndex < _WallSections; _CurrentIndex++)
		{
			if (GetWallsSettings().IsValidIndex(_CurrentIndex))
			{
				// SET THE DEFAULT SETTINGS ON THIS WALL SECTION IF IS NOT CONFIGURED
				if (!GetWallsSettings()[_CurrentIndex].WallMesh->IsValidLowLevel() && GetDefaultWallSetting().WallMesh->IsValidLowLevel())
				{
					GetWallsSettings()[_CurrentIndex].WallMesh = GetDefaultWallSetting().WallMesh;
				}

				if (!GetWallsSettings()[_CurrentIndex].PillarMesh->IsValidLowLevel() && GetDefaultWallSetting().PillarMesh->IsValidLowLevel())
				{
					GetWallsSettings()[_CurrentIndex].PillarMesh = GetDefaultWallSetting().PillarMesh;
				}

				// WALLS GENERATION
				if (GetWallsSettings()[_CurrentIndex].WallMesh->IsValidLowLevel())
				{
					FVector _WallStart = GetWallsSpline()->GetLocationAtSplinePoint(_CurrentIndex, ESplineCoordinateSpace::Local);
					FVector _WallEnd = GetWallsSpline()->GetLocationAtSplinePoint(_CurrentIndex + 1, ESplineCoordinateSpace::Local);
					float _WallSectionLength = FVector::Dist2D(_WallStart, _WallEnd);
					if (_WallSectionLength > 0)
					{
						FBoxSphereBounds _WallBounds = GetWallsSettings()[_CurrentIndex].WallMesh->GetBounds();
						float _WallSize = 199.9;// _WallBounds.BoxExtent.X * 2;
						int _WallsAmount = (_WallSectionLength / _WallSize) + 1;
						_WallsAmount = _WallsAmount == 0 ? 1 : _WallsAmount;
						float _WallScale = _WallSectionLength / (_WallSize * _WallsAmount);

						UInstancedStaticMeshComponent* _InstancedWall;
						if (GetInstanceByMesh(GetWallsSettings()[_CurrentIndex].WallMesh, _CreatedWalls, _CreatedPillars) != nullptr)
						{
							_InstancedWall = GetInstanceByMesh(GetWallsSettings()[_CurrentIndex].WallMesh, _CreatedWalls, _CreatedPillars);
						}
						else
						{
							_InstancedWall = Cast<UInstancedStaticMeshComponent>(AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, FTransform(), false));
							_InstancedWall->SetStaticMesh(GetWallsSettings()[_CurrentIndex].WallMesh);
							_CreatedWalls.Add(_InstancedWall);
						}

						for (int _CurrentWallIndex = 0; _CurrentWallIndex < _WallsAmount; _CurrentWallIndex++)
						{
							FTransform _WallTransform;
							_WallTransform.SetRotation(FQuat(GetWallsSpline()->GetRotationAtSplinePoint(_CurrentIndex, ESplineCoordinateSpace::Local)));
							_WallTransform.SetLocation(GetWallsSpline()->GetLocationAtSplinePoint(_CurrentIndex, ESplineCoordinateSpace::Local) + (_WallTransform.Rotator().Vector() * _WallScale * _WallSize * _CurrentWallIndex));
							_WallTransform.SetScale3D(FVector(_WallScale, 1, 1));
							_InstancedWall->AddInstance(_WallTransform);
						}
					}
				}
				
				if (GetWallsSettings()[_CurrentIndex].bAddPillar && GetWallsSettings()[_CurrentIndex].WallMesh->IsValidLowLevel())
				{
					UInstancedStaticMeshComponent* _InstancedPillar;
					if (GetInstanceByMesh(GetWallsSettings()[_CurrentIndex].PillarMesh, _CreatedWalls, _CreatedPillars) != nullptr)
					{
						_InstancedPillar = GetInstanceByMesh(GetWallsSettings()[_CurrentIndex].PillarMesh, _CreatedWalls, _CreatedPillars);
					}
					else
					{
						_InstancedPillar = Cast<UInstancedStaticMeshComponent>(AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, FTransform(), false));
						_InstancedPillar->SetStaticMesh(GetWallsSettings()[_CurrentIndex].PillarMesh);
						_CreatedPillars.Add(_InstancedPillar);
					}

					FTransform _WallTransform;
					_WallTransform.SetRotation(FQuat(GetWallsSpline()->GetRotationAtSplinePoint(_CurrentIndex, ESplineCoordinateSpace::Local)));
					_WallTransform.SetLocation(GetWallsSpline()->GetLocationAtSplinePoint(_CurrentIndex, ESplineCoordinateSpace::Local));
					_WallTransform.SetScale3D(FVector(1, 1, 1));
					_InstancedPillar->AddInstance(_WallTransform);
				}
			}
		}

	}
}

UInstancedStaticMeshComponent* AModularWall::GetInstanceByMesh(UStaticMesh* MeshReference, TArray<UInstancedStaticMeshComponent*> InstancedWalls, TArray<UInstancedStaticMeshComponent*> InstancedPillars)
{
	if (InstancedWalls.Num() > 0)
	{
		for (UInstancedStaticMeshComponent* _CurrentIndex : InstancedWalls)
		{
			if (_CurrentIndex->GetStaticMesh() == MeshReference)
			{
				return _CurrentIndex;
			}
		}
	}

	if (InstancedPillars.Num() > 0)
	{
		for (UInstancedStaticMeshComponent* _CurrentIndex : InstancedPillars)
		{
			if (_CurrentIndex->GetStaticMesh() == MeshReference)
			{
				return _CurrentIndex;
			}
		}
	}

	return nullptr;
}
