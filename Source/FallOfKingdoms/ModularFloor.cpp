// Fill out your copyright notice in the Description page of Project Settings.


#include "ModularFloor.h"
#include "Components/SplineComponent.h"
#include "Components/SceneComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
AModularFloor::AModularFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	FloorSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Floor Spline"));

	RootComponent = RootScene;
	FloorSpline->SetupAttachment(RootScene);
}

// Called when the game starts or when spawned
void AModularFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AModularFloor::OnConstruction(const FTransform& Transform)
{
	UInstancedStaticMeshComponent* _GeneratedFloor;
	_GeneratedFloor = Cast<UInstancedStaticMeshComponent>(AddComponentByClass(UInstancedStaticMeshComponent::StaticClass(), false, FTransform(), false));

	if (GetFloorMesh() != nullptr)
	{
		_GeneratedFloor->SetStaticMesh(GetFloorMesh());

		if (GetFloorSpline()->GetNumberOfSplinePoints() > 1)
		{
			FVector2D _FloorSize = FVector2D(199.9, 199.9);// FVector2D(GetFloorMesh()->GetBounds().BoxExtent * 2);
			float _LengthX = GetFloorSpline()->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local).X;
			float _LengthY = GetFloorSpline()->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local).Y;
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
					_InstanceTransform.SetLocation(FVector((_FloorSize.X * _FloorScaleX * _CurrentX), (_FloorSize.Y * _FloorScaleY * _CurrentY), 0));
					_InstanceTransform.SetScale3D(FVector(_FloorScaleX, _FloorScaleY, 1));
					_GeneratedFloor->AddInstance(_InstanceTransform);
				}
			}
		}
	}
}
