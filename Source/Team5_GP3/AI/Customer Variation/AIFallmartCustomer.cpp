#include "AIFallmartCustomer.h"
#include "Team5_GP3/Managers/GameManager.h"
#include "Team5_GP3/AI/CustomerComponent.h"
#include "Team5_GP3/AI/Others/StoreItem.h"
AAIFallmartCustomer::AAIFallmartCustomer() 
{
	
	//CustomerComponent = CreateDefaultSubobject<UCustomerComponent>(TEXT("CustomerComponent"));
}
AStoreItem* AAIFallmartCustomer::StoreItem()
{
	return nullptr;
}
void AAIFallmartCustomer::BeginPlay()
{
	//FallmartCustomerController = Cast<AAIFallmartCustomerController>(GetController());
}
void AAIFallmartCustomer::Tick(float DeltaTime)
{

}
//void AAIFallmartCustomer::DestroyActor()
//{
	//for (AStoreItem* StoreItem : CustomerComponent->ItemList)
	//{
	//	if (StoreItem != nullptr)
	//		StoreItem->Destroy();
	//}
//	Destroy();
//}
bool AAIFallmartCustomer::SetShopItem()
{
	//if (!mapLoaded) return mapLoaded;

	//ItemToGet = GM->PopAStoreItem(); 

	//CustomerComponent->ItemList.Add(ItemToGet);

	return ItemToGet != nullptr;
}