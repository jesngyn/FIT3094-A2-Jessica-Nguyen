// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GOAPAction.h"
#include "FIT3094_A2_Code/VillageCentreActor.h"

/**
 * 
 */
class AVillageCentreActor;
class FIT3094_A2_CODE_API DropOffWoodAction : public GOAPAction
{
public:
	DropOffWoodAction();
	~DropOffWoodAction();
	bool IsActionDone() override;

	virtual bool CheckProcedurePrecondition(AGOAPActor* Agent) override;

	virtual bool PerformAction(AGOAPActor* Agent) override;

	virtual bool RequiresInRange() override;

private:
	virtual void Reset() override;

	int WoodDropped;

	AVillageCentreActor* VillageCentre;

	//1 = 1 second - timer incrementing
	const int64 WoodDropTimer = 1;

	int64 TargetTime;

	//amount of resources to drop before action is complete
	const int WoodToDrop = 50;
};