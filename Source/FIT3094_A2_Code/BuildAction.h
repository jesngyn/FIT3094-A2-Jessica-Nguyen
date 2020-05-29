// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GOAPAction.h"
#include "CoreMinimal.h"
#include "FIT3094_A2_Code/BuildingActor.h"

/**
 * 
 */
class ABuildingActor;

class FIT3094_A2_CODE_API BuildAction : public GOAPAction
{
public:
	BuildAction();
	~BuildAction();

	bool IsActionDone() override;

	virtual bool CheckProcedurePrecondition(AGOAPActor* Agent) override;

	virtual bool PerformAction(AGOAPActor* Agent) override;

	virtual bool RequiresInRange() override;

private:
	virtual void Reset() override;

	int BuildWood;
	int BuildStone;

	ABuildingActor* BuildingActor;

	//1 = 1 second - timer incrementing
	const int64 BuildTimer = 2;

	int64 TargetTime;

	//amount of resources to gather before action is complete
	const int WoodToBuild = 10;
	const int StoneToBuild = 5;
};
