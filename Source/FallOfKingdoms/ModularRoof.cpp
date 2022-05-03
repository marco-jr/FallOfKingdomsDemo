// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularRoof.h"
#include "Components/SplineComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
AModularRoof::AModularRoof()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root"));
	RoofSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Roof Spline"));

	RootComponent = SceneRoot;
	RoofSpline->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AModularRoof::BeginPlay()
{
	Super::BeginPlay();
	
}

void AModularRoof::OnConstruction(const FTransform& Transform)
{
	if (GetRoofSpline()->GetNumberOfSplinePoints() > 1 && GetRoofMesh() != nullptr)
	{
		// SETUP DEFAULT SETTINGS
		float _SplineEndX = GetRoofSpline()->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local).X;
		float _SplineEndY = GetRoofSpline()->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local).Y;

		float _RoofStartY = 0;

		float _RoofLengthX = 199.9;// GetRoofMesh()->GetBounds().BoxExtent.X * 2;
		float _RoofLengthY = 199.9;// GetRoofMesh()->GetBounds().BoxExtent.Y * 2;

		int _RoofAmount = (_SplineEndY / _RoofLengthY) + 1;

		UInstancedStaticMeshComponent* _CreatedRoof;

		_CreatedRoof = Cast<UInstancedStaticMeshComponent>(AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, FTransform(), false));
		_CreatedRoof->SetStaticMesh(GetRoofMesh());

		FTransform _RoofTransform;
		_RoofTransform.SetScale3D(FVector(
			/* X */ _SplineEndX / _RoofLengthX / 2,
			/* Y */ _SplineEndY / (_RoofLengthY * _RoofAmount),
			/* Z */ 1));

		if (GetRoofEdgeMesh() != nullptr)
		{
			float _RoofEdgeLengthX = 199.9;// GetRoofEdgeMesh()->GetBounds().BoxExtent.X * 2;
			float _RoofEdgeLengthY = 199.9;// GetRoofEdgeMesh()->GetBounds().BoxExtent.Y * 2;
			
			UInstancedStaticMeshComponent* _CreatedRoofEdge;
			_CreatedRoofEdge = Cast<UInstancedStaticMeshComponent>(AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, FTransform(), false));
			_CreatedRoofEdge->SetStaticMesh(GetRoofEdgeMesh());

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
			_CreatedRoofEdge->AddInstance(_RoofEdgeTransform);

			// ADD UP LEFT EDGE ROOF
			_RoofEdgeTransform.SetScale3D(FVector(_ScaleY, _ScaleX, 1));
			_RoofEdgeTransform.SetRotation(FQuat(FRotator(0, 90, 0)));
			_RoofEdgeTransform.SetLocation(FVector(_SplineEndX, 0, 0));
			_CreatedRoofEdge->AddInstance(_RoofEdgeTransform);

			//ADD UP RIGHT EDGE ROOF
			_RoofEdgeTransform.SetScale3D(FVector(_ScaleX, _ScaleY, 1));
			_RoofEdgeTransform.SetRotation(FQuat(FRotator(0, 180, 0)));
			_RoofEdgeTransform.SetLocation(FVector(_SplineEndX, _SplineEndY, 0));
			_CreatedRoofEdge->AddInstance(_RoofEdgeTransform);

			//ADD DOWN RIGHT EDGE ROOF
			_RoofEdgeTransform.SetScale3D(FVector(_ScaleY, _ScaleX, 1));
			_RoofEdgeTransform.SetRotation(FQuat(FRotator(0, 270, 0)));
			_RoofEdgeTransform.SetLocation(FVector(0, _SplineEndY, 0));
			_CreatedRoofEdge->AddInstance(_RoofEdgeTransform);
		}

		if (_RoofAmount > 0)
		{
			float _ScaleX = _RoofTransform.GetScale3D().X;
			float _ScaleY = _RoofTransform.GetScale3D().Y;

			for (int _CurrentIndexY = 0; _CurrentIndexY < _RoofAmount; _CurrentIndexY++)
			{
				for (int _CurrentIndexX = 0; _CurrentIndexX < 2; _CurrentIndexX++)
				{
					UE_LOG(LogTemp, Warning, TEXT("Index: %d"), _CurrentIndexX);

					_RoofTransform.SetLocation(FVector(
						/* X */  _SplineEndX * _CurrentIndexX
						/* Y */, _RoofStartY + (_CurrentIndexY * _RoofLengthY * _ScaleY) + (_CurrentIndexX * _RoofLengthY * _ScaleY)
						/* Z */, 0
					));
					_RoofTransform.SetRotation(FQuat(FRotator(
						/* PITCH */  0
						/* YAW   */, _CurrentIndexX * 180
						/* ROLL  */, 0
					)));
					_CreatedRoof->AddInstance(_RoofTransform);
				}
			}
		}
	}
}
