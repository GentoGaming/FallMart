// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGrabComponent.h"

#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Team5_GP3/AI/AICustomerAbstract.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Team5_GP3/AI/Customer Variation/AIChildCustomer.h"
#include "Team5_GP3/Player/PlayerCharacter.h"

// Sets default values for this component's properties
UPlayerGrabComponent::UPlayerGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



void UPlayerGrabComponent::SetThrowingCustomer(bool condition)
{
	isFlying = condition;
	if(isFlying) ChildThrowDone();
	if(!condition)ReleaseReset();
}

void UPlayerGrabComponent::ChooseTargetToGrab()
{
	if(isFlying) return;
	
	if(CloneCustomerToThrow == nullptr)
	{
		PhysicsHandle->SetTargetLocation(GetOwner()->GetActorLocation());
		TArray<AActor*> ignoredActors;
		TArray<FHitResult> OutHits;
		UKismetSystemLibrary::SphereTraceMulti(GetWorld(), GetOwner()->GetActorLocation()  + GetOwner()->GetActorForwardVector() * 50,
											GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * grabbingSphereDistance, grabbingSphereRadius, ETraceTypeQuery::TraceTypeQuery1, false,
											ignoredActors, EDrawDebugTrace::None, OutHits, true,
											FLinearColor::Red, FLinearColor::Green, 6.0f);
		for (FHitResult Hit : OutHits)
		{

			if (Hit.GetActor() && Hit.GetActor()->IsA(AAICustomerAbstract::StaticClass()))
			{
				AAICustomerAbstract* AICustTemp = Cast<AAICustomerAbstract>(Hit.GetActor());
				// always true as Hit.GetActor is Customer
				if(AICustTemp)
				{
					if(AICustTemp->GetMesh()->IsSimulatingPhysics()) return;

					FVector Location = AICustTemp->GetActorLocation();
					FRotator Rotation =  AICustTemp->GetActorRotation();
					FActorSpawnParameters SpawnInfo;

					AAIChildCustomer *Child = Cast<AAIChildCustomer>(AICustTemp);
					OriginalCustomerToThrow = AICustTemp;
					
					if( Child !=nullptr )
					{
						AAICustomerAbstract* newCustomer =	GetWorld()->SpawnActor<AAICustomerAbstract>(CustomerChildBPClone,Location, Rotation, SpawnInfo);
						CloneCustomerToThrow = newCustomer;
					}else
					{
						AAICustomerAbstract* newCustomer =	GetWorld()->SpawnActor<AAICustomerAbstract>(CustomerBPClone,Location, Rotation, SpawnInfo);
						CloneCustomerToThrow = newCustomer;
					}

					if(!CloneCustomerToThrow)
					{
						OriginalCustomerToThrow = nullptr;
						return;

					}
					
					CloneCustomerToThrow->GetMesh()->SetSkeletalMesh(OriginalCustomerToThrow->GetMesh()->SkeletalMesh);

					int counter = 0;

					for(UMaterialInterface* UIMaterial : OriginalCustomerToThrow->GetMesh()->GetMaterials())
					{
						CloneCustomerToThrow->GetMesh()->SetMaterial(counter,UIMaterial);
						counter++;
					}
					
					CloneCustomerToThrow->Outfit->SetSkeletalMesh(OriginalCustomerToThrow->Outfit->SkeletalMesh);

					counter = 0;
					for(UMaterialInterface* UIMaterial : OriginalCustomerToThrow->Outfit->GetMaterials())
					{
						CloneCustomerToThrow->Outfit->SetMaterial(counter,UIMaterial);
						counter++;
					}
					
					CloneCustomerToThrow->Hair->SetStaticMesh(OriginalCustomerToThrow->Hair->GetStaticMesh());

					counter = 0;
					for(UMaterialInterface* UIMaterial : OriginalCustomerToThrow->Hair->GetMaterials())
					{
						CloneCustomerToThrow->Hair->SetMaterial(counter,UIMaterial);
						counter++;
					}

					CloneCustomerToThrow->PutClothOn();
					
					OriginalCustomerToThrow->SetActorLocation(FVector(5000,5000,5000));
					OriginalCustomerToThrow->DeleteAllItems();
					CloneCustomerToThrow->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					LiftCustomer();
					return;
				}
			}
		}
	}else
	{
		ChildThrowDone();
		Player->OnCustomerLanded(CloneCustomerToThrow);
	}

	
}

void UPlayerGrabComponent::ChildThrowDone() const
{
	AAIChildCustomer *Child = Cast<AAIChildCustomer>(CloneCustomerToThrow);
	if( Child !=nullptr )
	{
		Child->SetChildToThrown(); 
	}
}

// Called when the game starts
void UPlayerGrabComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UPlayerGrabComponent::LiftCustomer()
{
	//CustomerHeldByPlayer->GetCharacterMovement()->GravityScale = 0;
	const FVector NewLocationVector = GetOwner()->GetActorLocation() + ( (GrabLocationVector.X * GetOwner()->GetActorForwardVector()) + (GrabLocationVector.Z * GetOwner()->GetActorUpVector()) );
	CloneCustomerToThrow->GetMesh()->SetSimulatePhysics(true);
	PhysicsHandle->GrabComponentAtLocationWithRotation(CloneCustomerToThrow->GetMesh(),BoneName,CloneCustomerToThrow->GetActorLocation(),CloneCustomerToThrow->GetActorRotation());
}

void UPlayerGrabComponent::OnlyRelease()
{
	PhysicsHandle->ReleaseComponent();

}

void UPlayerGrabComponent::ReleaseReset()
{
	PhysicsHandle->ReleaseComponent();
	CloneCustomerToThrow = nullptr;
	PhysicsHandle->SetTargetLocation(GetOwner()->GetActorLocation());
}


// Called every frame
void UPlayerGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(isFlying) return;
	const FVector NewLocationVector = GetOwner()->GetActorLocation() + ( (GrabLocationVector.X * GetOwner()->GetActorForwardVector()) + (GrabLocationVector.Z * GetOwner()->GetActorUpVector()) );
	PhysicsHandle->SetTargetLocation(NewLocationVector);	
}

