#include "Team5_GP3/AI/BTServices/BTServiceShopItemLocation.h"
#include "Team5_GP3/AI/Customer Variation/AIFallmartCustomer.h"
UBTServiceShopItemLocation::UBTServiceShopItemLocation()
{
	NodeName = "GetStoreItemLocation";
}
void UBTServiceShopItemLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaTime);

	AAIFallmartCustomer* AICustomer = 
		Cast<AAIFallmartCustomer>(OwnerComp.GetOwner()->GetInstigator());
	if (AICustomer == nullptr)
		return;
	if (AICustomer->StoreItem() == nullptr)
		return;
	
	UE_LOG(LogTemp, Log, TEXT("Customer is shopping"));
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), AICustomer->StoreItem()->GetActorLocation());
}