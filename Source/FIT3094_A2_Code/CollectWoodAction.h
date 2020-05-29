// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"
#include "FIT3094_A2_Code/TreeActor.h"

/**
 * 
 */
class ATreeActor;

class FIT3094_A2_CODE_API CollectWoodAction : public GOAPAction
{
public:
	CollectWoodAction();
	~CollectWoodAction();

	bool IsActionDone() override;

	virtual bool CheckProcedurePrecondition(AGOAPActor* Agent) override;

	virtual bool PerformAction(AGOAPActor* Agent) override;

	virtual bool RequiresInRange() override;

private:
	virtual void Reset() override;
	
	int WoodGathered;

	TArray<ATreeActor*> TreeList;
	
	//1 = 1 second - timer incrementing
	const int64 WoodTimer = 1;

	int64 TargetTime;

	//amount of resources to gather before action is complete
	const int WoodToGather = 50;
};
