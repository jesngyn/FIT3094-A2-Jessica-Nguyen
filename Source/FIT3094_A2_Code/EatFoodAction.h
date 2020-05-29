// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"
#include "FIT3094_A2_Code/FoodActor.h"

/**
 * 
 */
class AFoodActor;

class FIT3094_A2_CODE_API EatFoodAction : public GOAPAction
{
public:
	EatFoodAction();
	~EatFoodAction();

	bool IsActionDone() override;

	virtual bool CheckProcedurePrecondition(AGOAPActor* Agent) override;

	virtual bool PerformAction(AGOAPActor* Agent) override;

	virtual bool RequiresInRange() override;

private:
	virtual void Reset();

	int FoodGathered;

	TArray<AFoodActor*> FoodList;

	//1 = 1 second - timer incrementing
	const int64 FoodTimer = 2;

	int64 TargetTime;

	//amount of resources to gather before action is complete
	const int FoodToGather = 1;
};

	