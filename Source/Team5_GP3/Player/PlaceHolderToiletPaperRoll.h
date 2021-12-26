// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "PlaceHolderToiletPaperRoll.generated.h"

class USphereComponent;
class UStaticMeshComponent;
UCLASS()
class TEAM5_GP3_API APlaceHolderToiletPaperRoll : public AActor
{
	GENERATED_BODY()
private:
	FVector PlayerForward;
	float LifeTimeTimer;
	float lerpSpeed = 0;
public:
	UPROPERTY(EditAnywhere)
	float Speed;
	UPROPERTY(EditAnywhere)
	float SphereTraceRadius;
	UPROPERTY(EditAnywhere)
	float LifeTime;
	UPROPERTY(EditAnywhere)
	USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	APlaceHolderToiletPaperRoll();
	virtual void Tick(float DeltaTime) override;

	void Throw(FVector force);
	// UFUNCTION(BlueprintImplementableEvent)
	// void OnTPHitFighter();


};
