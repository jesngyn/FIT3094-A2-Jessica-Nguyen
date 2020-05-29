// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FIT3094_A2_Code/StateMachine.h"
#include "Containers/Queue.h"
#include "FIT3094_A2_Code/CollectWoodAction.h"
#include "FIT3094_A2_Code/CollectStoneAction.h"
#include "FIT3094_A2_Code/CollectBuildMatAction.h"
#include "FIT3094_A2_Code/DropOffWoodAction.h"
#include "FIT3094_A2_Code/DropOffStoneAction.h"
#include "FIT3094_A2_Code/BuildAction.h"
#include "GOAPActor.generated.h"

UCLASS()
class FIT3094_A2_CODE_API AGOAPActor : public AActor
{
	GENERATED_BODY()
	
public:	
	enum Actor_States
	{
		State_Nothing,
		State_Idle,
		State_Move,
		State_Action
	};
	
	const float Tolerance = 20;
	float MoveSpeed;

	// Sets default values for this actor's properties
	AGOAPActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	StateMachine<Actor_States, AGOAPActor>* ActionStateMachine;

	TSet<GOAPAction*> AvailableActions;
	TQueue<GOAPAction*> CurrentActions;

	//State machine functions
	void OnIdleEnter();
	void OnIdleUpdate(float DeltaTime);
	void OnIdleExit();

	void OnMoveEnter();
	void OnMoveUpdate(float DeltaTime);
	void OnMoveExit();

	void OnActionEnter();
	void OnActionUpdate(float DeltaTime);
	void OnActionExit();

	// Timed function that decreases health every 2 seconds;
	void DecreaseHealth();

	// Handle for Timer
	FTimerHandle TimerHandle;
	

	virtual TMap<FString, bool> GetWorldState();
	virtual TMap<FString, bool> CreateGoalState();

	virtual void OnPlanFailed(TMap<FString, bool> FailedGoalState);
	virtual void OnPlanAborted(GOAPAction* FailedAction);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
