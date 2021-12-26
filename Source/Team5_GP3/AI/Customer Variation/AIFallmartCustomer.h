#pragma once
#include "Team5_GP3/AI/AIFallmartCustomerAbstract.h"
#include "TEam5_GP3/AI/Others/StoreItem.h"
#include "AIFallmartCustomer.generated.h"
class UCustomerComponent;
class UBlackboardComponent;
class UGameManager;
struct FItemProperty;
UCLASS()
class AAIFallmartCustomer : public AAIFallmartCustomerAbstract 
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	AAIFallmartCustomer();
	UFUNCTION(BlueprintCallable)
	AStoreItem* StoreItem();
	bool SetShopItem();
//	UFUNCTION(BlueprintCallable)
//	void DestroyActor() override;
	//UPROPERTY()
	//UBlackboardComponent* AIFallmartCustomerBlackboard;
	//UPROPERTY()
	//UGameManager* GM; //TODO: Change to more appropriate name, "GM" to avoid linker error
	AStoreItem* ItemToGet;
	bool mapLoaded = false;
	void SetWalkGoal();
};