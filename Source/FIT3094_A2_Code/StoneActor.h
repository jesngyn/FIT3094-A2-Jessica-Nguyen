// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "StoneActor.generated.h"

UCLASS()
class FIT3094_A2_CODE_API AStoneActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;
	
public:	
	// Sets default values for this actor's properties
	AStoneActor();

	UPROPERTY(EditAnywhere, Category = "Stone")
		int StoneResources = 150;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
