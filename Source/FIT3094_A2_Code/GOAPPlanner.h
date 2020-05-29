// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Queue.h"
#include "CoreMinimal.h"

class AGOAPActor;
class GOAPAction;

/**
 * 
 */
struct GOAPNode
{
	GOAPNode* Parent;
	float RunningCost;
	TMap<FString, bool> State;
	GOAPAction* Action;
};

class FIT3094_A2_CODE_API GOAPPlanner
{
public:
	GOAPPlanner();
	~GOAPPlanner();

	static bool Plan(AGOAPActor* Agent, const TSet<GOAPAction*>& AvailableActions, TQueue<GOAPAction*>& PlannedActions, TMap<FString, bool> WorldState, TMap<FString, bool> GoalState);

protected:
	static bool BuildGraphRecursive(TArray<GOAPNode*>& AllNodes, GOAPNode* Parent, TArray<GOAPNode*>& GoalNodes, const TSet<GOAPAction*>& AvailableActions, TMap<FString, bool>& GoalState);

	static TSet<GOAPAction*> CreateActionSubset(const TSet<GOAPAction*>& AvailableActions, GOAPAction* RemoveAction);

	static bool CheckConditionsInState(TMap<FString, bool>& Conditions, TMap<FString, bool>& State);

	static TMap <FString, bool> PopulateNewState(const TMap<FString, bool>& CurrentState, TMap<FString, bool>& Changes);
};
