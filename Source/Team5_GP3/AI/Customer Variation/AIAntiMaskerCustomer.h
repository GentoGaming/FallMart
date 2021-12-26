#pragma once

#include "CoreMinimal.h"
#include "../AICustomerAbstract.h"
#include "AIAntiMaskerCustomer.generated.h"
class APlayerCharacter;
class UAIPerceptionComponent;
/**
 * 
 */
UCLASS()
class TEAM5_GP3_API AAIAntiMaskerCustomer : public AAICustomerAbstract
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
public:
	AAIAntiMaskerCustomer();
	void AvoidPlayer();
	UFUNCTION(BlueprintNativeEvent)
	void RunAwayFromPlayer(bool runAway);

	UFUNCTION(BlueprintImplementableEvent)
	void OnTakeOffMask();
	UFUNCTION(BlueprintImplementableEvent)
	void OnPutMask(); 
	UFUNCTION(BlueprintImplementableEvent)
	void OnTickBroken();
	
	void DoneShopping();
	bool FindValidLocation(FVector& SafeLocation);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactToPlayer")
	bool PlayerToClose;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactToPlayer")
	float FleeFromPlayerMinDistance = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactToPlayer")
	float FleeFromPlayerMaxDistance = 1000.f;
	UPROPERTY(EditAnywhere, Category = "ReactToPlayer")
	UAIPerceptionComponent* CustomerPerceptionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ReactToPlayer")
	float FindSafeLocationAngleInterval = 10.f;
	UPROPERTY(EditAnywhere, Category = "ReactToPlayer")
	float AllowedDistanceFromFleePlayerLocation = 100;

	FVector RightSide;
	FVector LeftSide;
	float capsuleRadius = 0;
};
