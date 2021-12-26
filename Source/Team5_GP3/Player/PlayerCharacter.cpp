#include "PlayerCharacter.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Team5_GP3/AI/AICustomerAbstract.h"
#include "Team5_GP3/AI/CustomerComponent.h"
#include "Team5_GP3/AI/Others/InfectionCloud.h"
#include "Team5_GP3/AI/Others/ThrowableItem.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MovementComponent = GetCharacterMovement();
	playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	SlapMaskCapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SlapTheMaskCapsule"));
	PlayerHand = CreateDefaultSubobject<USceneComponent>("Player Hand");
	PlayerHand->SetupAttachment(RootComponent);
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roll Area Of Hit"));
	SphereMesh->SetupAttachment(RootComponent);
	C_Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Roll Spline Path Component"));
	C_Spline->SetupAttachment(PlayerHand);
	SingleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Roll Single Mesh"));
	SingleMesh->SetupAttachment(PlayerHand);
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle"));
	PlayerGrabComponent= CreateDefaultSubobject<UPlayerGrabComponent>(TEXT("Player Grab Component"));
	Hair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
}

void APlayerCharacter::MoveForward(float input)
{
	if (!bCanMove) return;
	AddMovementInput(FVector::ForwardVector * input);
	// SetActorRotation(FRotator(GetVelocity().Normalize()));
}

void APlayerCharacter::MoveRight(float input)
{
	if (!bCanMove) return;
	AddMovementInput(FVector::RightVector * input);
}


void APlayerCharacter::RotatePlayerCharacter(bool isSlaping)
{
	FVector worldDirection;
	FVector mouseLocation;
	FCollisionQueryParams traceSettings;
	FCollisionResponseParams traceResponse;
	FHitResult HitResult;

	playerController->DeprojectMousePositionToWorld(mouseLocation, worldDirection);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, mouseLocation, (worldDirection * 10000.f) + mouseLocation,
	                                         ECollisionChannel::ECC_WorldStatic, traceSettings, traceResponse))
	{
		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.ImpactPoint);

		SetActorRotation(FRotator(GetActorRotation().Pitch, rotation.Yaw, GetActorRotation().Roll));
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		if (isSlaping)
		{
			OnLeftMouseClick.Broadcast();
		}
		else
		{
			if(!PlayerGrabComponent->isFlying)
			OnThrow.Broadcast();
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("did not hit a static object %s"), *worldDirection.ToString())
	}
}

void APlayerCharacter::ThrowingRollReleased()
{
	if(PlayerGrabComponent->isFlying) return;

	if(PlayerGrabComponent->CloneCustomerToThrow !=nullptr)
	{
		PlayerGrabComponent->SetThrowingCustomer(true);
		OnCustomerReleased.Broadcast(PlayerGrabComponent->CloneCustomerToThrow, PhysicsHandle);
	}else
	{
		OnThrowReleased.Broadcast();
	}
	}

void APlayerCharacter::DestroyInfected()
{
	TArray<AActor*> ignoredActors;
	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetActorLocation() + (GetActorForwardVector() * 250),
	                                       GetActorLocation() + (GetActorForwardVector() * destroyInfectionRadius), destroyInfectionRadius, ETraceTypeQuery::TraceTypeQuery1, false,
	                                       ignoredActors, EDrawDebugTrace::None, OutHits, true,
	                                       FLinearColor::Red, FLinearColor::Green, 6.0f);


	
	for (FHitResult Hit : OutHits)
	{
		if (Hit.GetActor() && Hit.GetActor()->IsA(AAICustomerAbstract::StaticClass()))
		{
			AAICustomerAbstract * CustomerAbstract = Cast<AAICustomerAbstract>(Hit.GetActor());
			if(CustomerAbstract)
			{
				CustomerAbstract->CustomerComponent->ResetInfection();
			}
		}

		if (Hit.GetActor() && Hit.GetActor()->IsA(AInfectionCloud::StaticClass()))
		{
			Hit.GetActor()->Destroy();
		}
	}
	
}


void APlayerCharacter::GrabTarget()
{
	
	PlayerGrabComponent->ChooseTargetToGrab();
	if(PlayerGrabComponent->CloneCustomerToThrow!=nullptr)
	{
		OnGrabSuccess();
	}
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	bCanMove = true;
	//Todo: should be moved to GameManager
	playerController->bShowMouseCursor = true;
	playerController->bEnableClickEvents = true;
	playerController->bEnableMouseOverEvents = true;
	PlayerGrabComponent->Player = this;
	PlayerGrabComponent->PhysicsHandle = PhysicsHandle;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!FMath::IsNearlyZero(GetVelocity().Size()))
	{
		SetActorRotation(GetVelocity().Rotation());
	}
}


// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("WalkForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("WalkRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("MousePosition", IE_Pressed, this,
	                                                       &APlayerCharacter::RotatePlayerCharacter, true);
	PlayerInputComponent->BindAction<FCustomInputDelegate>("RollThrowing", IE_Pressed, this,
	                                                       &APlayerCharacter::RotatePlayerCharacter, false);
	PlayerInputComponent->BindAction("RollThrowing", IE_Released, this, &APlayerCharacter::ThrowingRollReleased);
	PlayerInputComponent->BindAction("DestroyInfection", IE_Pressed, this, &APlayerCharacter::DestroyInfected);
	PlayerInputComponent->BindAction("GrabCustomer", IE_Pressed, this, &APlayerCharacter::GrabTarget);
}
