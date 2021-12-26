// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIAntiMaskerCustomer.h"
#include "AIChildCustomer.generated.h"

class UBoxComponent;
UCLASS()
class TEAM5_GP3_API AAIChildCustomer : public AAIAntiMaskerCustomer
{
	GENERATED_BODY()
	private:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	bool childIsDone = false;
	UPROPERTY()
	TArray<AActor*>AllActorsTriedToAttack;
	void StopAttacking();
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere, Category = "Child Distance")
	float childDistanceFromParent = 300.0f; // Child must be next to parent before Getting crazy Again.
	UPROPERTY(EditAnywhere, Category = "Child Distance")
	float takeMaskFromDistance = 100.0f; // Child must be next to parent before Getting crazy Again.
	UPROPERTY()
	AActor * Player;
	UPROPERTY()
	AAICustomerAbstract* Guardian ;
	UPROPERTY()
	AAICustomerAbstract* CustomerToAttack = nullptr;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	bool isStarted = false;
	public:
	AAIChildCustomer();
	void SetParent(AAICustomerAbstract* Parent){Guardian = Parent;}
	void CheckIfParentLeftStore();
	UPROPERTY(EditAnywhere, Category = "Child Movement Speed")
	float childWalkSpeed = 450;
	UPROPERTY(EditAnywhere, Category = "Child Movement Speed")
	float childChaseSpeed = 900;
	float childGotThrown = false;
	UFUNCTION(BlueprintCallable)
	void SetChildToThrown(){childGotThrown = true;};
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool isCrazy = false;
	UFUNCTION()
	void ToggleAndFireEvent(bool cond);
	UFUNCTION(BlueprintImplementableEvent)
	void OnChildCraziness(bool cond);
};
