#include "AIAntiMaskerCustomer.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Team5_GP3/AI/CustomerComponent.h"
#include "Team5_GP3/Managers/GameManager.h"
#include "GameFramework/CharacterMovementComponent.h"

AAIAntiMaskerCustomer::AAIAntiMaskerCustomer()
{
	PrimaryActorTick.bCanEverTick = true;
	CustomerPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("CustomerPerceptionComponent"));
	if (GameManager == nullptr)
	{

	}
	if (capsuleRadius == 0) capsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();

}

void AAIAntiMaskerCustomer::BeginPlay()
{
	Super::BeginPlay();
	if (capsuleRadius == 0) capsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	BlackboardComponent->SetValueAsFloat(TEXT("FleePlayerMinDistance"), FleeFromPlayerMinDistance);
	BlackboardComponent->SetValueAsFloat(TEXT("FleePlayerMaxDistance"), FleeFromPlayerMaxDistance);
	BlackboardComponent->SetValueAsInt(TEXT("ItemsToShopAmount"), itemsToShop);


}

void AAIAntiMaskerCustomer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!CustomerComponent->WearMask())
		AvoidPlayer();
	if (PlayerToClose && CustomerComponent->WearMask()) BlackboardComponent->SetValueAsBool(TEXT("PlayerTooClose"), false);
	
	OnTickBroken();
}

void AAIAntiMaskerCustomer::AvoidPlayer()
{
	if (!PlayerToClose && FVector::Dist(GetActorLocation(), GameManager->GetPlayerLocation()) < FleeFromPlayerMinDistance)
	{
		PlayerToClose = true;
	}
	else if (PlayerToClose && FVector::Dist(GetActorLocation(), GameManager->GetPlayerLocation()) > FleeFromPlayerMinDistance) PlayerToClose = false;

	BlackboardComponent->SetValueAsBool(TEXT("PlayerTooClose"), PlayerToClose);
	RunAwayFromPlayer(PlayerToClose);
}
void AAIAntiMaskerCustomer::RunAwayFromPlayer_Implementation(bool runAway) 
{
	
}
void AAIAntiMaskerCustomer::DoneShopping()
{
	if (GetAllPickedItems().Num() >= itemsToShop)
	{
		BlackboardComponent->SetValueAsBool(TEXT("FinnishedShopping"), GetAllPickedItems().Num() >= itemsToShop);
	}
}