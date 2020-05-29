// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
class AGOAPActor;

/**
 * 
 */
class FIT3094_A2_CODE_API GOAPAction
{
public:
	TMap<FString, bool> Preconditions;
	TMap<FString, bool> Effects;

	float ActionCost;
	AActor* Target;
	
	GOAPAction();
	virtual ~GOAPAction() = 0;

	void DoReset();

	void AddPrecondition(FString Name, bool State);
	void RemovePrecondition(FString Name);

	void AddEffect(FString Name, bool State);
	void RemoveEffect(FString Name);

	bool IsInRange() const { return InRange; }
	void SetInRange(const bool Range) { InRange = Range; }

	virtual bool IsActionDone() = 0;
	
	virtual bool CheckProcedurePrecondition(AGOAPActor* Agent) = 0;

	virtual bool PerformAction(AGOAPActor* Agent) = 0;

	virtual bool RequiresInRange() = 0;

private:
	bool InRange;

	virtual void Reset() = 0;
	
};
