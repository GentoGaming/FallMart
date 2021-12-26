// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMaskSlapComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Team5_GP3/AI/CustomerComponent.h"
#include "Team5_GP3/Player/PlayerCharacter.h"
#include "Team5_GP3/AI/CustomerComponent.h"

// Sets default values for this component's properties
UPlayerMaskSlapComponent::UPlayerMaskSlapComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("SlapCollider"));
	auto* player = GetOwner();
	Player = Cast<APlayerCharacter>(GetOwner());
}

void UPlayerMaskSlapComponent::OnSlapNativeEvent_Implementation()
{
}


// Called when the game starts
void UPlayerMaskSlapComponent::BeginPlay()
{
	Super::BeginPlay();
	CapsuleComponent = Player->SlapMaskCapsuleCollider;
	Player->OnLeftMouseClick.AddDynamic(this, &UPlayerMaskSlapComponent::SlapMask);
}


void UPlayerMaskSlapComponent::SlapMask()
{
	if (SlapCooldownTimer >= 0) return;
	UE_LOG(LogTemp, Log, TEXT(" :is Slaping"));
	FVector EndLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * SlapDistance;
	float halfHeight;
	float capsuleRadius;
	CapsuleComponent->GetScaledCapsuleSize(capsuleRadius, halfHeight);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Player);
	FHitResult HitResult;
	UKismetSystemLibrary::CapsuleTraceSingle(GetWorld(), Player->GetActorLocation(), EndLocation, capsuleRadius,
	                                         halfHeight, ETraceTypeQuery::TraceTypeQuery2, true, ActorsToIgnore,
	                                         EDrawDebugTrace::None, HitResult, true);

	if (HitResult.Actor != nullptr)
	{
		UCustomerComponent* CustomerComponent;
		UE_LOG(LogTemp, Log, TEXT("hit and has a UCustomerComponet, %s"), *HitResult.Actor->GetName());
		CustomerComponent = Cast<UCustomerComponent>(
			HitResult.GetActor()->FindComponentByClass(UCustomerComponent::StaticClass()));
		if (CustomerComponent != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("put on mask, %s"), *HitResult.Actor->GetName());
			SlapCooldownTimer = SlapCoolDown;
			FSlapData SlapData;
			SlapData.CustomerComponent = CustomerComponent;
			SlapData.SlapedCustomer = HitResult.GetActor();
			
			if (!CustomerComponent->HasMask)
			{
				CustomerComponent->PutOnMask();
				SlapData.SlapSucceed = true;
			}
			else
			{
				SlapData.SlapSucceed = false;
			}


			OnSlapMask.Broadcast(SlapData);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("hit actor without UCostomerComponent, %s"), *HitResult.Actor->GetName());
		}
	}
}

void UPlayerMaskSlapComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (SlapCooldownTimer >= 0)
		SlapCooldownTimer -= DeltaTime;
}
