// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerGrabComponent.generated.h"


class APlayerCharacter;
class UPhysicsHandleComponent;
class AAICustomerAbstract;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAM5_GP3_API UPlayerGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerGrabComponent();
	UPROPERTY()
	UPhysicsHandleComponent *PhysicsHandle;
	UPROPERTY()
	AAICustomerAbstract* CloneCustomerToThrow;
	UPROPERTY()
	AAICustomerAbstract* OriginalCustomerToThrow;
	
	UFUNCTION(BlueprintPure)
	AAICustomerAbstract* GetCustomerHeldByPlayer() const { return CloneCustomerToThrow; }

	UFUNCTION(BlueprintPure)
	AAICustomerAbstract* GetOriginalCustomer() const {return OriginalCustomerToThrow;};
	UPROPERTY()
	APlayerCharacter* Player;
	UFUNCTION(BlueprintCallable)
	void SetThrowingCustomer(bool condition);
	UFUNCTION(BlueprintCallable)
	void SetCustomerHeld(AAICustomerAbstract* CustomerAbsHeld) { CloneCustomerToThrow = CustomerAbsHeld; };
	void ChooseTargetToGrab();
	UPROPERTY(EditAnywhere, Category="GrabbingCustomer")
	FName BoneName = "head";
	UPROPERTY(EditAnywhere, Category="GrabbingCustomer")
	FVector GrabLocationVector = {70,0,0};
	UPROPERTY(EditAnywhere, Category="GrabbingCustomer")
	float grabbingSphereDistance = 200.f;

	UPROPERTY(EditAnywhere, Category="GrabbingCustomer")
	float grabbingSphereRadius = 200.f;
	FTransform HandlerTransform; 
	void ChildThrowDone() const;
	bool isFlying = false;
	//UFUNCTION(BlueprintCallable)
	//void DestroyClone();
	UPROPERTY(EditAnywhere, Category="GrabbingCustomer")
	TSubclassOf<AAICustomerAbstract> CustomerBPClone;
	UPROPERTY(EditAnywhere, Category="GrabbingCustomer")
	TSubclassOf<AAICustomerAbstract> CustomerChildBPClone;

	UFUNCTION(BlueprintCallable)
	void OnlyRelease();

	void ReleaseReset();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void LiftCustomer();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
