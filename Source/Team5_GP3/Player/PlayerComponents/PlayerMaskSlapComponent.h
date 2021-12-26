// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Team5_GP3/AI/CustomerComponent.h"
// #include "Team5_GP3/Player/PlayerCharacter.h"


#include "PlayerMaskSlapComponent.generated.h"

class APlayerCharacter;

USTRUCT(BlueprintType)
struct FSlapData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AActor* SlapedCustomer;
	UPROPERTY(BlueprintReadOnly)
	UCustomerComponent* CustomerComponent;
	UPROPERTY(BlueprintReadOnly)
	bool SlapSucceed;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSlapDelegate, const FSlapData&, FSlapData);

UCLASS(ClassGroup=(Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class TEAM5_GP3_API UPlayerMaskSlapComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	float SlapCoolDown;
	float SlapCooldownTimer;


public:
	UCapsuleComponent* CapsuleComponent;
	APlayerCharacter* Player;

	// UPROPERTY(EditAnywhere)
	// TEnumAsByte<ETraceTypeQuery> TraceTypeQuery;
	UPROPERTY(EditAnywhere)
	float SlapDistance;


public:
	// Sets default values for this component's properties
	UPlayerMaskSlapComponent();
	UFUNCTION(BlueprintNativeEvent)
	void OnSlapNativeEvent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable)
	FSlapDelegate OnSlapMask;

	UFUNCTION()
	void SlapMask();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
