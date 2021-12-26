// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Team5_GP3/AI/AICustomerAbstract.h"
#include "Team5_GP3/AI/Others/ThrowableItem.h"

#include "PlayerThrowRollComponent.generated.h"


class UPhysicsHandleComponent;
class USplineMeshComponent;
class APlayerCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAM5_GP3_API UPlayerThrowRollComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerThrowRollComponent();
	UPROPERTY()
	APlayerCharacter* Player;
	UPROPERTY(EditAnywhere, Category="Casting Roll Settings")
	TSubclassOf<AThrowableItem> RollToThrow;
	UPROPERTY(EditAnywhere, Category="Casting Roll Settings")
	TSubclassOf<AThrowableItem> RollToThrowCustomer;
	UPROPERTY(EditAnywhere, Category="Casting Roll Settings")
	float minForce = 300;
	UPROPERTY(EditAnywhere, Category="Casting Roll Settings")
	float maxForce = 2000;
	UPROPERTY(EditAnywhere, Category="Casting Roll Settings")
	float powerSpeed = 600.f;
	float force;
	FVector rollVelocity;
	UPROPERTY()
	TArray<USplineMeshComponent *> A_SplineMesh;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void DrawProjectory();
	UFUNCTION()
	void ClearSplineMeshes();
	UFUNCTION()
	void StopDrawProjectory();

	UFUNCTION()
	void StopDrawCustomerProjectory(AAICustomerAbstract * CustomerToThrow, UPhysicsHandleComponent* PhysicsHandleComponent);
	
	FTimerHandle handle;
	bool isDrawProjectory = false;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
