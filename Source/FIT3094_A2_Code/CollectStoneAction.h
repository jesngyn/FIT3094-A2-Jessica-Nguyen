// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"
#include "FIT3094_A2_Code/StoneActor.h"


/**
 * 
 */
class AStoneActor;

class FIT3094_A2_CODE_API CollectStoneAction : public GOAPAction
{
public:
	CollectStoneAction();
	~CollectStoneAction();

	bool IsActionDone() override;

	virtual bool CheckProcedurePrecondition(AGOAPActor* Agent) override;

	virtual bool PerformAction(AGOAPActor* Agent) override;

	virtual bool RequiresInRange() override;

private:
	virtual void Reset() override;

	int StoneGathered;

	TArray<AStoneActor*> StoneList;

	//1 = 1 second - timer incrementing
	const int64 StoneTimer = 3;

	int64 TargetTime;

	//amount of resources to gather before action is complete
	const int StoneToGather = 20;
};