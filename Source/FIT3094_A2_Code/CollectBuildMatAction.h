// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"
#include "FIT3094_A2_Code/VillageCentreActor.h"

/**
 * 
 */
class AVillageCentreActor;

class FIT3094_A2_CODE_API CollectBuildMatAction : public GOAPAction
{
public:
	CollectBuildMatAction();
	~CollectBuildMatAction();
	
	bool IsActionDone() override;

	virtual bool CheckProcedurePrecondition(AGOAPActor* Agent) override;

	virtual bool PerformAction(AGOAPActor* Agent) override;

	virtual bool RequiresInRange() override;

private:
	virtual void Reset() override;

	int StoneGathered;
	int WoodGathered;

	AVillageCentreActor* VillageCentre;
	
	//1 = 1 second - timer incrementing
	const int64 CollectionTimer = 2;

	int64 TargetTime;

	//amount of resources to gather before action is complete
	const int WoodToGather = 10;
	const int StoneToGather = 5;
};


