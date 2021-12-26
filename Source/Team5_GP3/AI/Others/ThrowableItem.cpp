#include "ThrowableItem.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Team5_GP3/AI/FighterComponent.h"
#include "Team5_GP3/AI/Customer Variation/AIFighterCustomer.h"
#include "Team5_GP3/Managers/GameManager.h"
#include "Team5_GP3/Player/PlayerCharacter.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Team5_GP3/AI/AICustomerAbstract.h"

AThrowableItem::AThrowableItem()
{
	PrimaryActorTick.bCanEverTick = true;
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
}

void AThrowableItem::HitTheGround()
{
	// This is a workaround , Or call it a failSafe . If the roll is thrown and no collision happened As Art forgot some collision 
	// The Z kill will Be triggered and this function will be called	
	isThrowingACustomer = false;
	Player->OnCustomerLanded(CustomerThrowing);
	Destroy();

	
}

void AThrowableItem::ZKillTriggered()
{
	isThrowingACustomer = false;
	Player->OnCustomerLanded(CustomerThrowing);
}

void AThrowableItem::BeginPlay()
{
	Super::BeginPlay();
	if (!isCustomerRoll)
	{
		this->OnActorHit.AddDynamic(this, &AThrowableItem::OverlapFighterOnCollision);
		// SetLifeSpan(20); // Just for now but this will be gone and replace with destroy on collision with floor or actors 
	}

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AThrowableItem::ThrowCustomer(const FVector& Velocity, AAICustomerAbstract* CustomerToThrow, UPhysicsHandleComponent* PhysicsHandleComponent)
{
	CustomerThrowing = CustomerToThrow;
	PhysicsHandle = PhysicsHandleComponent;
	isThrowingACustomer = true;
	ProjectileMovementComponent->SetVelocityInLocalSpace(Velocity);
}

void AThrowableItem::OverlapFighterOnCollision(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse,
                                               const FHitResult& Hit)
{
	// UE_LOG(LogTemp, Display, TEXT("Overlap is in method"));

	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), OverlapFighterRadius, ObjectTypeList,
	                                              ClassFilter, ActorsToIgnore, OutActors))
	{
		for (auto Actor : OutActors)
		{
			AAIController* aiController = Cast<AAIController>(Actor->GetOwner());
			if (aiController->GetBlackboardComponent()->GetValueAsBool("IsFighting"))
			{
				UFighterComponent* FighterComponent = Cast<UFighterComponent>(
					Actor->GetComponentByClass(UFighterComponent::StaticClass()));


				UFighterComponent* otherFighterPartnerComponent = FighterComponent->GetFightingPartner();

				if(otherFighterPartnerComponent == nullptr) continue;
				AAIController* partnerAiController = Cast<AAIController>(
					Cast<AAIFighterCustomer>(otherFighterPartnerComponent->GetOwner())->GetController());

				if (partnerAiController)
				{
					UBlackboardComponent* partnerBlackBoard = partnerAiController->GetBlackboardComponent();
					if (partnerBlackBoard)
					{
						aiController->GetBlackboardComponent()->
						              SetValueAsBool("IsFighting", false);
						aiController->GetBlackboardComponent()->SetValueAsBool("HasFought", true);
						partnerBlackBoard->SetValueAsBool("IsFighting", false);
						partnerBlackBoard->SetValueAsBool("HasFought", true);
						FighterComponent->OnStopFighting.Broadcast();
						otherFighterPartnerComponent->OnStopFighting.Broadcast();

						OnHitGround();
						Destroy();
						return;
					}
				}
				
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("ai is not fighting"));
			}
		}
	}
	OnHitGround();
	Destroy();
}

void AThrowableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(isThrowingACustomer)
	{
		if(PhysicsHandle)
		{
			PhysicsHandle->SetTargetLocation(GetActorLocation());
		}
	}
}

void AThrowableItem::Throw(FVector Velocity)
{
	ProjectileMovementComponent->SetVelocityInLocalSpace(Velocity);
}


// AAIController* aiController = Cast<AAIController>(OutActors[0]->GetOwner());
// if (aiController->GetBlackboardComponent()->GetValueAsBool("IsFighting"))
// {
// 	aiController->GetBlackboardComponent()->
//                   SetValueAsBool("IsFighting", false);
// 	aiController->GetBlackboardComponent()->SetValueAsBool("HasFought", true);
//
//
// 	UFighterComponent* otherFighterComponent = Cast<UFighterComponent>(
//         OutActors[0]->GetComponentByClass(UFighterComponent::StaticClass()));
// 	otherFighterComponent->OnStopFighting.Broadcast();
// 	UFighterComponent* otherFighterPartner = otherFighterComponent->GetFightingPartner();
// 	otherFighterPartner->OnStopFighting.Broadcast();
// 	AAIController* partnerAiController = Cast<AAIController>(
//         Cast<AAIFighterCustomer>(otherFighterPartner->GetOwner())->GetController());
// 	if (partnerAiController)
// 	{
// 		UBlackboardComponent* partnerBlackBoard = partnerAiController->GetBlackboardComponent();
// 		if (partnerBlackBoard)
// 		{
// 			partnerBlackBoard->SetValueAsBool("IsFighting", false);
// 			partnerBlackBoard->SetValueAsBool("HasFought", true);
// 		}
// 	}
//
// }
// else
// {
// 	UE_LOG(LogTemp, Display, TEXT("ai is not fighting"));
// }
