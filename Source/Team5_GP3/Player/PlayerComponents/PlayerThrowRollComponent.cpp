// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerThrowRollComponent.h"

#include <activation.h>

#include "DrawDebugHelpers.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Team5_GP3/Player/PlayerCharacter.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UPlayerThrowRollComponent::UPlayerThrowRollComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPlayerThrowRollComponent::BeginPlay()
{
	Super::BeginPlay();
	isDrawProjectory = false;
	Player = Cast<APlayerCharacter>(GetOwner());
	Player->OnThrow.AddDynamic(this, &UPlayerThrowRollComponent::DrawProjectory);
	Player->OnThrowReleased.AddDynamic(this, &UPlayerThrowRollComponent::StopDrawProjectory);
	Player->OnCustomerReleased.AddDynamic(this, &UPlayerThrowRollComponent::StopDrawCustomerProjectory);

	ClearSplineMeshes();
}

void UPlayerThrowRollComponent::DrawProjectory()
{
	Player->bCanMove = false;
	Player->SphereMesh->SetVisibility(true);
	force = minForce;
	isDrawProjectory = true;
}

void UPlayerThrowRollComponent::ClearSplineMeshes()
{

	for (USplineMeshComponent *spline : A_SplineMesh)
	{
		spline->DestroyComponent();
	}

	Player->C_Spline->ClearSplinePoints(true);

	A_SplineMesh.Empty();
}

void UPlayerThrowRollComponent::StopDrawProjectory()
{
	FVector Location = Player->PlayerHand->GetComponentLocation();
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	AThrowableItem *Roll = GetWorld()->SpawnActor<AThrowableItem>(RollToThrow,Location, Rotation, SpawnInfo);
	Roll->Throw(rollVelocity);
	isDrawProjectory = false;
	Player->bCanMove = true;
	Player->SphereMesh->SetVisibility(false);
	ClearSplineMeshes();
}

void UPlayerThrowRollComponent::StopDrawCustomerProjectory(AAICustomerAbstract* CustomerToThrow, UPhysicsHandleComponent* PhysicsHandleComponent)
{
//	Player->SetCustomerHeld(nullptr);
	FVector Location = Player->PlayerHand->GetComponentLocation();
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	AThrowableItem *Roll = GetWorld()->SpawnActor<AThrowableItem>(RollToThrowCustomer,Location, Rotation, SpawnInfo);
	//Roll->Throw(rollVelocity);
	Roll->ThrowCustomer(rollVelocity,CustomerToThrow,PhysicsHandleComponent);
	isDrawProjectory = false;
	Player->bCanMove = true;
	Player->SphereMesh->SetVisibility(false);
	ClearSplineMeshes();
}


// Called every frame
void UPlayerThrowRollComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);	
	if(isDrawProjectory)
	{
	ClearSplineMeshes();
	FVector HandLocation = Player->PlayerHand->GetComponentLocation();			
	force =	FMath::FInterpConstantTo(force,maxForce,DeltaTime,powerSpeed);
	FVector HandForwardVector = Player->PlayerHand->GetForwardVector() * force;
	rollVelocity = HandForwardVector;
		FPredictProjectilePathResult  PredictResult;
		FPredictProjectilePathParams PredictParams( 1.f, HandLocation, HandForwardVector, 2.0f,
			UEngineTypes::ConvertToObjectType( ECC_WorldStatic ) );
		PredictParams.SimFrequency = 4;

		bool PredictionCheck =  UGameplayStatics::PredictProjectilePath(GetWorld(),PredictParams,PredictResult);

		Player->SphereMesh->SetWorldLocation(PredictResult.LastTraceDestination.Location,false,nullptr,ETeleportType::None);

		for( int index = 0;index < PredictResult.PathData.Num(); index++)
		{
			Player->C_Spline->AddSplinePointAtIndex(PredictResult.PathData[index].Location,index,ESplineCoordinateSpace::World,true);
		}

		int lastIndex = PredictResult.PathData.Num() - 1 ;
		Player->C_Spline->SetSplinePointType(lastIndex,ESplinePointType::CurveClamped, true);

		int spineNumber =Player->C_Spline->GetNumberOfSplinePoints() -2;

		
		for( int index = 0; index < spineNumber ; index++)
		{
			USplineMeshComponent *splineMesh = NewObject<USplineMeshComponent>(this);
			splineMesh->CreationMethod = EComponentCreationMethod::UserConstructionScript;
			FVector locStart, locEnd, tanStart,tanEnd ;
			
			Player->C_Spline->GetLocationAndTangentAtSplinePoint(index, locStart, tanStart, ESplineCoordinateSpace::World);
			Player->C_Spline->GetLocationAndTangentAtSplinePoint(index+1, locEnd, tanEnd, ESplineCoordinateSpace::World);
			
			splineMesh->SetForwardAxis(ESplineMeshAxis::Z);
			splineMesh->SetStaticMesh(Player->SingleMesh->GetStaticMesh());
			splineMesh->SetMaterial(0, Player->SingleMesh->GetMaterial(0));
			
			splineMesh->SetMobility(EComponentMobility::Type::Movable);
			splineMesh->SetStartAndEnd(locStart, tanStart, locEnd, tanEnd);
			splineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			splineMesh->SetStartScale(FVector2D{0.07,0.07});
			splineMesh->SetEndScale(FVector2D{0.07,0.07});
			splineMesh->AttachToComponent(Player->C_Spline, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

			splineMesh->bMeshDirty = true;
			splineMesh->UpdateMesh();
			splineMesh->RegisterComponent();

			A_SplineMesh.Add(splineMesh);
		}

		Player->C_Spline->UpdateSpline();
	}
	

}


