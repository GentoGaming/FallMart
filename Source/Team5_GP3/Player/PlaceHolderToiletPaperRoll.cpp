// Fill out your copyright notice in the Description page of Project Settings.
#include "PlaceHolderToiletPaperRoll.h"
#include "Components/SphereComponent.h"
// Sets default values
APlaceHolderToiletPaperRoll::APlaceHolderToiletPaperRoll()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// RootComponent = Root;
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	RootComponent = SphereComponent;
	// SphereComponent->SetupAttachment(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TPMesh"));
	Mesh->SetupAttachment(SphereComponent);
}

// Called when the game starts or when spawned
void APlaceHolderToiletPaperRoll::BeginPlay()
{
	Super::BeginPlay();
	PlayerForward = GetOwner()->GetActorForwardVector();
	SphereComponent->AddImpulse(Speed * PlayerForward, "None", true);
	SetLifeSpan(LifeTime);
}


// Called every frame
void APlaceHolderToiletPaperRoll::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	
}

void APlaceHolderToiletPaperRoll::Throw(FVector force)
{
	// SphereComponent->AddImpulse( Speed * PlayerForward);
}
