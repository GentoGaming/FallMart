#include "CustomerComponent.h"
#include "Customer Variation/AIAntiMaskerCustomer.h"

UCustomerComponent::UCustomerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCustomerComponent::ResetInfection()
{
	isInfected = false;
	if(Customer)Customer->OnInfected(false);;
	latestTickTime = 0;
	firstTickTimeInfected = 0;
}


void UCustomerComponent::BeforeDestroyed() const
{
	if(HasMask || !GameManager) return;
		if (GameManager != nullptr)
		GameManager->OnThreatChange(-ThreatDmg) ;
}

void UCustomerComponent::ActivateThreat(UGameManager* GameM)
{
	GameManager = GameM;
	
	if(!HasMask && GameManager!= nullptr)
	{
			GameManager->OnThreatChange(ThreatDmg);	
	}
}

void UCustomerComponent::SetInfected(bool cond)
{
	isInfected = cond;
	if(Customer)Customer->OnInfected(cond);;
}

void UCustomerComponent::PutOnMask()
{
	
	if (GameManager != nullptr)
	{
		GameManager->OnThreatChange(-ThreatDmg);
	}
	
	HasMask = true;
	
	AAIAntiMaskerCustomer *AntiMasker = Cast<AAIAntiMaskerCustomer>(GetOwner());
	if(AntiMasker!=nullptr) AntiMasker->OnPutMask();
}

void UCustomerComponent::TakeOffMask()
{

	if (GameManager != nullptr)
	{
		GameManager->OnThreatChange(ThreatDmg);
	}
	
	HasMask = false;
	AAIAntiMaskerCustomer *AntiMasker = Cast<AAIAntiMaskerCustomer>(GetOwner());
	if(AntiMasker!=nullptr) AntiMasker->OnTakeOffMask();
}

void UCustomerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(isInfected)
	{
		const float timeNowIs  = GetWorld()->GetTimeSeconds();
		
		if(firstTickTimeInfected ==0) firstTickTimeInfected = timeNowIs;
		if(latestTickTime == 0) latestTickTime = timeNowIs;

		
	if(timeNowIs - firstTickTimeInfected > maxLifeSpanInfectionCloud)
	{
		isInfected = false;
		if(Customer)Customer->OnInfected(false);;

		latestTickTime = 0;
		firstTickTimeInfected = 0;
		return;
	}

	if(timeNowIs - latestTickTime > coolDown )
	{
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		const FActorSpawnParameters SpawnInfo;
		GetWorld()->SpawnActor<AInfectionCloud>(infectionCloud,GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation(), SpawnInfo);
		latestTickTime = timeNowIs;
		coolDown = FMath::RandRange(minCoughTime, maxCoughTime);
	}	
	}
}

void UCustomerComponent::BeginPlay()
{
	Super::BeginPlay();
	firstTickTimeInfected = 0;
	latestTickTime = 0;
	coolDown = FMath::RandRange(minCoughTime, maxCoughTime);
	Customer = Cast<AAICustomerAbstract>(GetOwner());

}

bool UCustomerComponent::WearMask() const
{
	return HasMask;
}
