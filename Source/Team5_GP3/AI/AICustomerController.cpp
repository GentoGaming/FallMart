#include "AICustomerController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "DetourCrowdAIController.h"
AAICustomerController::AAICustomerController(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
	

}