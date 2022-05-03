// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularSpawner.h"
#include "ModularWall.h"
#include "ModularFloor.h"
#include "ModularRoof.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"

// Sets default values
AModularSpawner::AModularSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AModularSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// GENERATE WALLS
	TArray<AActor*> _FoundWalls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AModularWall::StaticClass(), _FoundWalls);

	if (_FoundWalls.Num() > 0)
	{
		CreateWalls(_FoundWalls);
	}

	// GENERATE FLOORS
	TArray<AActor*> _FoundFloors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AModularFloor::StaticClass(), _FoundFloors);

	if (_FoundFloors.Num() > 0)
	{
		CreateFloors(_FoundFloors);
	}

	// GENERATE ROOFS
	TArray<AActor*> _FoundRoofs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AModularRoof::StaticClass(), _FoundRoofs);

	if (_FoundRoofs.Num() > 0)
	{
		CreateRoofs(_FoundRoofs);
	}
}

UHierarchicalInstancedStaticMeshComponent* AModularSpawner::GetCreatedInstanceByMesh(UStaticMesh* MeshReference)
{
	for (UHierarchicalInstancedStaticMeshComponent* _CurrentIndex : GetCreatedInstances())
	{
		if (_CurrentIndex->GetStaticMesh() == MeshReference)
		{
			return _CurrentIndex;
		}
	}

	UHierarchicalInstancedStaticMeshComponent* _CreatedInstance = Cast<UHierarchicalInstancedStaticMeshComponent>(AddComponentByClass(UHierarchicalInstancedStaticMeshComponent::StaticClass(), false, FTransform(), false));
	_CreatedInstance->SetStaticMesh(MeshReference);
	CreatedInstances.Add(_CreatedInstance);

	return _CreatedInstance;
}

void AModularSpawner::CreateWalls(TArray<AActor*> WallReferences)
{
	for (AActor* _CurrentActor : WallReferences)
	{
		if (AModularWall* _CurrentWall = Cast<AModularWall>(_CurrentActor))
		{
			// DEFAULT SETTINGS
			int _WallSections = _CurrentWall->GetWallsSpline()->GetNumberOfSplinePoints() - 1;

			// GENERATE WALLS
			if (_WallSections > 0)
			{
				for (int _CurrentIndex = 0; _CurrentIndex < _WallSections; _CurrentIndex++)
				{
					if (_CurrentWall->GetWallsSettings().IsValidIndex(_CurrentIndex))
					{
						if (_CurrentWall->GetWallsSettings()[_CurrentIndex].WallMesh->IsValidLowLevel())
						{
							FVector _WallStart = _CurrentWall->GetWallsSpline()->GetLocationAtSplinePoint(_CurrentIndex, ESplineCoordinateSpace::World);
							FVector _WallEnd = _CurrentWall->GetWallsSpline()->GetLocationAtSplinePoint(_CurrentIndex + 1, ESplineCoordinateSpace::World);
							float _WallSectionLength = FVector::Dist2D(_WallStart, _WallEnd);
							if (_WallSectionLength > 0)
							{
								FBoxSphereBounds _WallBounds = _CurrentWall->GetWallsSettings()[_CurrentIndex].WallMesh->GetBounds();
								float _WallSize = 199.9;// _WallBounds.BoxExtent.X * 2;
								int _WallsAmount = (_WallSectionLength / _WallSize) + 1;
								_WallsAmount = _WallsAmount == 0 ? 1 : _WallsAmount;
								float _WallScale = _WallSectionLength / (_WallSize * _WallsAmount);

								UStaticMesh* _WallMesh = _CurrentWall->GetWallsSettings()[_CurrentIndex].WallMesh;
								UHierarchicalInstancedStaticMeshComponent* _CurrentInstancedWall = GetCreatedInstanceByMesh(_WallMesh);

								for (int _CurrentWallIndex = 0; _CurrentWallIndex < _WallsAmount; _CurrentWallIndex++)
								{
									FTransform _WallTransform;
									_WallTransform.SetRotation(FQuat(_CurrentWall->GetWallsSpline()->GetRotationAtSplinePoint(_CurrentIndex, ESplineCoordinateSpace::World)));
									_WallTransform.SetLocation(_CurrentWall->GetWallsSpline()->GetLocationAtSplinePoint(_CurrentIndex, ESplineCoordinateSpace::World) + (_WallTransform.Rotator().Vector() * _WallScale * _WallSize * _CurrentWallIndex));
									_WallTransform.SetScale3D(FVector(_WallScale, 1, 1));
									_CurrentInstancedWall->AddInstance(_WallTransform);
								}
							}
						}

						if (_CurrentWall->GetWallsSettings()[_CurrentIndex].bAddPillar && _CurrentWall->GetWallsSettings()[_CurrentIndex].WallMesh->IsValidLowLevel())
						{

							UStaticMesh* _PillarMesh = _CurrentWall->GetWallsSettings()[_CurrentIndex].PillarMesh;
							UHierarchicalInstancedStaticMeshComponent* _CurrentInstancedPillar = GetCreatedInstanceByMesh(_PillarMesh);

							FTransform _WallTransform;
							_WallTransform.SetRotation(FQuat(_CurrentWall->GetWallsSpline()->GetRotationAtSplinePoint(_CurrentIndex, ESplineCoordinateSpace::World)));
							_WallTransform.SetLocation(_CurrentWall->GetWallsSpline()->GetLocationAtSplinePoint(_CurrentIndex, ESplineCoordinateSpace::World));
							_WallTransform.SetScale3D(FVector(1, 1, 1));
							_CurrentInstancedPillar->AddInstance(_WallTransform);
						}
					}
				}
			}
			_CurrentWall->Destroy();
		}
	}
}

void AModularSpawner::CreateFloors(TArray<AActor*> FloorReferences)
{
	if (FloorReferences.Num() > 0)
	{
		for (AActor* _CurrentActor : FloorReferences)
		{
			AModularFloor* _CurrentFloor = Cast<AModularFloor>(_CurrentActor);

			UHierarchicalInstancedStaticMeshComponent* _FloorInstance = GetCreatedInstanceByMesh(_CurrentFloor->FloorMesh);
			
			if (_CurrentFloor->GetFloorSpline()->GetNumberOfSplinePoints() > 1 && _CurrentFloor->GetFloorMesh() != nullptr)
			{
				FVector2D _FloorSize = FVector2D(199.9, 199.9);// FVector2D(int(_CurrentFloor->GetFloorMesh()->GetBounds().BoxExtent.X * 2), int(_CurrentFloor->GetFloorMesh()->GetBounds().BoxExtent.Y * 2));
				float _LengthX = _CurrentFloor->GetFloorSpline()->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local).X;
				UE_LOG(LogTemp, Warning, TEXT("%f"), _LengthX);
				float _LengthY = _CurrentFloor->GetFloorSpline()->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local).Y;
				int _FloorAmountX = _LengthX / _FloorSize.X;
				_FloorAmountX = _FloorAmountX == 0 ? 1 : _FloorAmountX + 1;
				int _FloorAmountY = _LengthY / _FloorSize.Y;
				_FloorAmountY = _FloorAmountY == 0 ? 1 : _FloorAmountY + 1;
				float _FloorScaleX = _LengthX / (_FloorAmountX * _FloorSize.X);
				float _FloorScaleY = _LengthY / (_FloorAmountY * _FloorSize.Y);

				for (int _CurrentX = 0; _CurrentX < _FloorAmountX; _CurrentX++)
				{
					for (int _CurrentY = 0; _CurrentY < _FloorAmountY; _CurrentY++)
					{
						FTransform _InstanceTransform;
						_InstanceTransform.SetLocation(_CurrentFloor->GetFloorSpline()->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World) + FVector((_FloorSize.X * _FloorScaleX * _CurrentX), (_FloorSize.Y * _FloorScaleY * _CurrentY), 0));
						_InstanceTransform.SetScale3D(FVector(_FloorScaleX, _FloorScaleY, 1));
						_FloorInstance->AddInstance(_InstanceTransform);
					}
				}
			}
			_CurrentFloor->Destroy();
		}
	}
}

void AModularSpawner::CreateRoofs(TArray<AActor*> RoofReferences)
{
	for (AActor* _CurrentActor : RoofReferences)
	{
		if (AModularRoof* _CurrentRoof = Cast<AModularRoof>(_CurrentActor))
		{
			if (_CurrentRoof->GetRoofSpline()->GetNumberOfSplinePoints() > 1 && _CurrentRoof->GetRoofMesh() != nullptr)
			{
				// SETUP DEFAULT SETTINGS
				float _SplineEndX = _CurrentRoof->GetRoofSpline()->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local).X;
				float _SplineEndY = _CurrentRoof->GetRoofSpline()->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local).Y;

				float _RoofStartY = 0;

				float _RoofLengthX = 199.9;// _CurrentRoof->GetRoofMesh()->GetBounds().BoxExtent.X * 2;
				float _RoofLengthY = 199.9;// _CurrentRoof->GetRoofMesh()->GetBounds().BoxExtent.Y * 2;

				int _RoofAmount = (_SplineEndY / _RoofLengthY) + 1;

				FTransform _RoofTransform;
				_RoofTransform.SetScale3D(FVector(
					/* X */ _SplineEndX / (_RoofLengthX * 2),
					/* Y */ _SplineEndY / (_RoofLengthY * _RoofAmount),
					/* Z */ 1));


				if (_CurrentRoof->GetRoofEdgeMesh() != nullptr)
				{
					float _RoofEdgeLengthX = 199.9;// _CurrentRoof->GetRoofEdgeMesh()->GetBounds().BoxExtent.X * 2;
					float _RoofEdgeLengthY = 199.9;// _CurrentRoof->GetRoofEdgeMesh()->GetBoundingBox().GetExtent().Y * 2;

					UHierarchicalInstancedStaticMeshComponent* _CreatedRoofEdge = GetCreatedInstanceByMesh(_CurrentRoof->GetRoofEdgeMesh()); Cast<UInstancedStaticMeshComponent>(AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, FTransform(), false));

					float _ScaleX = _SplineEndX / (_RoofEdgeLengthX * 2);
					float _ScaleY = _SplineEndY / (_RoofEdgeLengthY * 2);
					_ScaleY = _ScaleY > 1 ? 1 : _ScaleY;

					float _RoofUsableLength = _SplineEndY - _RoofEdgeLengthY * 2;
					_RoofAmount = _RoofUsableLength > 0 ? _RoofUsableLength / _RoofLengthY + 1 : 0;
					_RoofStartY = _RoofEdgeLengthY * _ScaleY;
					_RoofTransform.SetScale3D(FVector(
						/* X */ _SplineEndX / (_RoofLengthX * 2),
						/* Y */ _RoofUsableLength / (_RoofLengthY * _RoofAmount),
						/* Z */ 1));

					FTransform _RoofEdgeTransform;

					// ADD DOWN LEFT EDGE ROOF
					_RoofEdgeTransform.SetScale3D(FVector(_ScaleX, _ScaleY, 1));
					_RoofEdgeTransform.SetRotation(FQuat(_CurrentRoof->GetActorRotation()));
					_RoofEdgeTransform.SetLocation(_CurrentRoof->GetActorLocation());
					_CreatedRoofEdge->AddInstance(_RoofEdgeTransform);

					// ADD UP LEFT EDGE ROOF
					_RoofEdgeTransform.SetScale3D(FVector(_ScaleY, _ScaleX, 1));
					_RoofEdgeTransform.SetRotation(FQuat(FRotator(0, 90, 0) + _CurrentRoof->GetActorRotation()));
					_RoofEdgeTransform.SetLocation(_CurrentRoof->GetActorLocation() + (_CurrentRoof->GetActorForwardVector() * _SplineEndX));
					_CreatedRoofEdge->AddInstance(_RoofEdgeTransform);
					
					//ADD UP RIGHT EDGE ROOF
					_RoofEdgeTransform.SetScale3D(FVector(_ScaleX, _ScaleY, 1));
					_RoofEdgeTransform.SetRotation(FQuat(FRotator(0, 180, 0) + _CurrentRoof->GetActorRotation()));
					_RoofEdgeTransform.SetLocation(_CurrentRoof->GetActorLocation() + (_CurrentRoof->GetActorForwardVector() * _SplineEndX) + (_CurrentRoof->GetActorRightVector() * _SplineEndY));
					_CreatedRoofEdge->AddInstance(_RoofEdgeTransform);

					//ADD DOWN RIGHT EDGE ROOF
					_RoofEdgeTransform.SetScale3D(FVector(_ScaleY, _ScaleX, 1));
					_RoofEdgeTransform.SetRotation(FQuat(FRotator(0, 270, 0) + _CurrentRoof->GetActorRotation()));
					_RoofEdgeTransform.SetLocation(_CurrentRoof->GetActorLocation() + (_CurrentRoof->GetActorRightVector() * _SplineEndY));
					_CreatedRoofEdge->AddInstance(_RoofEdgeTransform);
					
				}


				if (_RoofAmount > 0)
				{
					UHierarchicalInstancedStaticMeshComponent * _CreatedRoof = GetCreatedInstanceByMesh(_CurrentRoof->GetRoofMesh()); Cast<UInstancedStaticMeshComponent>(AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, FTransform(), false));

					float _ScaleX = _RoofTransform.GetScale3D().X;
					float _ScaleY = _RoofTransform.GetScale3D().Y;

					for (int _CurrentIndexY = 0; _CurrentIndexY < _RoofAmount; _CurrentIndexY++)
					{
						for (int _CurrentIndexX = 0; _CurrentIndexX < 2; _CurrentIndexX++)
						{
							_RoofTransform.SetLocation(_CurrentRoof->GetActorLocation() + (_CurrentRoof->GetActorRightVector() * _RoofStartY) + (_CurrentRoof->GetActorForwardVector() * _CurrentIndexX * _ScaleX * _RoofLengthX * 2) + (_CurrentRoof->GetActorRightVector() * _CurrentIndexY * _ScaleY * _RoofLengthY) + (_CurrentRoof->GetActorRightVector() * _ScaleY * _RoofLengthY * _CurrentIndexX));
							_RoofTransform.SetRotation(FQuat(FRotator(
								/* PITCH */  0
								/* YAW   */, _CurrentRoof->GetActorRotation().Yaw + _CurrentIndexX * 180
								/* ROLL  */, 0
							)));
							_CreatedRoof->AddInstance(_RoofTransform);
						}
					}
				}
			}
			_CurrentRoof->Destroy();
		}
	}
}
