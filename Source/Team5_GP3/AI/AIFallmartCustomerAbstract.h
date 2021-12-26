#pragma once
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIFallmartCustomerAbstract.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UBlackboardComponent;
class UCharacterMovementComponent;
class AAIFallmartCustomerController;
class UGameManager;
UCLASS(Abstract)
class AAIFallmartCustomerAbstract : public ACharacter
{
	GENERATED_BODY()
public:

	AAIFallmartCustomerAbstract();
	UPROPERTY()
		AAIFallmartCustomerController* FallmartCustomerController;
	UPROPERTY(EditAnywhere, meta = (Category = "Behavior"))
	UBehaviorTree* AIBehaviorTree;
	UPROPERTY()
		UGameManager* GM;
	UPROPERTY(EditAnywhere)
	UBlackboardComponent* CustomerBlackBoard;

	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetBehavior() const { return AIBehaviorTree; };
	virtual void DestroyActor() {};
	virtual void SetWalkGoalPosition() {};

};