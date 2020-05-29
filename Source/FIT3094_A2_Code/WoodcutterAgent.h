// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPActor.h"
#include "Components/StaticMeshComponent.h"
#include "FIT3094_A2_Code/CollectWoodAction.h"
#include "FIT3094_A2_Code/DropOffWoodAction.h"
#include "FIT3094_A2_Code/EatFoodAction.h"
#include "TimerManager.h"
#include "WoodcutterAgent.generated.h"

class CollectWoodAction;
class DropOffWoodAction;
class EatFoodAction;

UCLASS()
class FIT3094_A2_CODE_API AWoodcutterAgent : public AGOAPActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* VisibleComponent;
	
public:	
	// Sets default values for this actor's properties
	AWoodcutterAgent();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Timed function that decreases health every 2 seconds;
	void DecreaseHealth();

	// Handle for Timer
	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TMap<FString, bool> GetWorldState() override;
	TMap<FString, bool> CreateGoalState() override;


	UPROPERTY(EditAnywhere)
		int NumWood;

	const int maxHealth = 50;

	UPROPERTY(EditAnywhere)
		int health;
};
