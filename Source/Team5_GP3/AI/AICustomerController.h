#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICustomerController.generated.h"

UCLASS()
class TEAM5_GP3_API AAICustomerController : public AAIController
{
	GENERATED_BODY()
public:
	AAICustomerController(const FObjectInitializer& ObjectInitializer);
	AAIController* controller;
	
};
