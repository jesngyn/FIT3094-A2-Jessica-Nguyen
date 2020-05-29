// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingActor.generated.h"

UCLASS()
class FIT3094_A2_CODE_API ABuildingActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;

public:	
	// Sets default values for this actor's properties
	ABuildingActor();

	UPROPERTY(BlueprintReadWrite, Category = "Resources")
		int StoneResources = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Resources")
		int WoodResources = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
