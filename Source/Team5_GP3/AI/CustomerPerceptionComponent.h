#pragma once
#include "Perception/AIPerceptionComponent.h"
#include "CustomerPerceptionComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class TEAM5_GP3_API UCustomerPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
		UCustomerPerceptionComponent();
	UFUNCTION(BlueprintCallable)
	void DetectPlayer();
	UFUNCTION(BlueprintCallable)
	void DetectObstacles();
	FActorPerceptionInfo PercievedActorInfo;
};