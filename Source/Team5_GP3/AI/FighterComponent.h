// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
// #include "Customer Variation/AIFighterCustomer.h"

#include "FighterComponent.generated.h"

class AAIFighterCustomer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFightingDelegate);

UCLASS(ClassGroup=(Custom), BlueprintType, meta=(BlueprintSpawnableComponent))
class TEAM5_GP3_API UFighterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFighterComponent();
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Fighter")
	FFightingDelegate OnFighting;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Fighter")
	FFightingDelegate OnStopFighting;

	UPROPERTY(EditAnywhere)
	
	bool IsFighting;
	
	UPROPERTY(EditAnywhere)
	UFighterComponent* FightingPartner;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetFightingPartner(UFighterComponent* OtherFighterCustomer);

	UFUNCTION(BlueprintCallable)
	UFighterComponent* GetFightingPartner();
};
