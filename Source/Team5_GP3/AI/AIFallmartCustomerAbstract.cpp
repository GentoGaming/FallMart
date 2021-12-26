#include "AIFallmartCustomerAbstract.h"
#include "Team5_GP3/AI/AIFallmartCustomerController.h"
#include "Team5_GP3/Managers/GameManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
AAIFallmartCustomerAbstract::AAIFallmartCustomerAbstract()
{
	PrimaryActorTick.bCanEverTick;
	//GM = Cast<UGameManager>(GetGameInstance());
	FallmartCustomerController = Cast<AAIFallmartCustomerController>(GetController());

	
}