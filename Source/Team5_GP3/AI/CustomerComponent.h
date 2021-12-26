#pragma once
#include "Components/ActorComponent.h"
#include "Others/InfectionCloud.h"
#include "Team5_GP3/AI/Others/StoreItem.h"
#include "Team5_GP3/Managers/GameManager.h"

#include "CustomerComponent.generated.h"
//class AItemsAbstract;
//Made to be put on a customer
USTRUCT()
struct FItemProperty
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	AStoreItem* Items; // TODO: Check for better solution, pointer is dangerous
	UPROPERTY(EditAnywhere)
	bool HasItem;
	UPROPERTY(EditAnywhere)
	FVector ItemLocation;
};

UCLASS(ClassGroup=(Custom), BlueprintType, meta=(BlueprintSpawnableComponent))
class UCustomerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCustomerComponent();
	UPROPERTY(EditAnywhere)
	TSubclassOf<AInfectionCloud> infectionCloud;
	void ResetInfection();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasMask; /** check if mask*/
	UPROPERTY(EditAnywhere)
	float ThreatDmg = 5;
	// will be -X when it leaves the supermarket or put mak on . and increase to + X when mask is off
	UPROPERTY(EditAnywhere)
	bool HasChild; //If true, when done, a child will spawn with customer and player has to find it 
	UPROPERTY(EditAnywhere)
	TArray<AStoreItem*> ItemList; //List of items for the customer to get	
	UPROPERTY()
	UGameManager* GameManager;
	
	UPROPERTY(EditAnywhere)
	bool isInfected = false;

	bool gettingAttackedByAChild;

	UFUNCTION(BlueprintCallable)
	void SetInfected(bool cond);
	void PutOnMask();
	void TakeOffMask();
virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;
	bool WearMask() const;
	void BeforeDestroyed() const;
	void ActivateThreat(UGameManager* GameM);
private:
	float latestTickTime;
	float firstTickTimeInfected;
	UPROPERTY()
	AAICustomerAbstract* Customer;
	float coolDown = 8;
	UPROPERTY(EditAnywhere,Category="CoughSettings | Infection")
	float minCoughTime = 4;
	UPROPERTY(EditAnywhere,Category="CoughSettings | Infection")
	float maxCoughTime = 8;
	UPROPERTY(EditAnywhere,Category="CoughSettings | Infection")
	float maxLifeSpanInfectionCloud = 40;

};
