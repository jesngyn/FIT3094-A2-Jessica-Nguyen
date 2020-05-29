// Fill out your copyright notice in the Description page of Project Settings.


#include "GOAPPlanner.h"
#include "FIT3094_A2_Code/GOAPAction.h"

GOAPPlanner::GOAPPlanner()
{
}

GOAPPlanner::~GOAPPlanner()
{
}


bool GOAPPlanner::Plan(AGOAPActor* Agent, const TSet<GOAPAction*>& AvailableActions, TQueue<GOAPAction*>& PlannedActions, TMap<FString, bool> WorldState, TMap<FString, bool> GoalState)
{
	//create array to hold all nodes during planning
	TArray<GOAPNode*> AllNodes;

	//reset all action states
	for (auto Action : AvailableActions)
	{
		Action->DoReset();
	}

	//clear planned actions
	PlannedActions.Empty();

	//get usable actions
	TSet<GOAPAction*> UsableActions;
	for (auto Action : AvailableActions)
	{
		if (Action->CheckProcedurePrecondition(Agent))
		{
			UsableActions.Add(Action);
		}
	}

	TArray<GOAPNode*> GoalNodes;

	//define starting node
	GOAPNode* Start = new GOAPNode;
	Start->Parent = nullptr;
	Start->RunningCost = 0;
	Start->State = WorldState;
	Start->Action = nullptr;

	//call build graph function
	//true if goal found, false if not
	bool IsSuccessful = BuildGraphRecursive(AllNodes, Start, GoalNodes, AvailableActions, GoalState);

	//if not successful then return
	if (!IsSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("GOAPPLANNER: No Plan Found"));
		return false;
	}

	//find cheapest goal node
	GOAPNode* CheapestNode = nullptr;
	for(auto Node : GoalNodes)
	{
		if (CheapestNode)
		{
			if (Node->RunningCost < CheapestNode->RunningCost)
			{
				CheapestNode = Node;
			}
		} else
		{
			CheapestNode = Node;
		}
	}
	//work back from cheapest goal to start
	TArray<GOAPAction*> Results;
	GOAPNode* CurrentNode = CheapestNode;

	//while currentnode's action isnt null then insert into result list, if it is null then it is the start node
	while (CurrentNode)
	{
		if (CurrentNode->Action)
		{
			Results.Insert(CurrentNode->Action, 0);
		}
		CurrentNode = CurrentNode->Parent;
	}

	//building queue from results
	for (auto Action : Results)
	{
		PlannedActions.Enqueue(Action);
	}

	//clean up nodes
	AllNodes.Empty();

	//succeed
	return true;
}

bool GOAPPlanner::BuildGraphRecursive(TArray<GOAPNode*>& AllNodes, GOAPNode* Parent, TArray<GOAPNode*>& GoalNodes, const TSet<GOAPAction*>& AvailableActions, TMap<FString, bool>& GoalState)
{
	bool HasFoundGoal = false;

	//for every action in available actions,
	for (auto Action: AvailableActions)
	{
		//check preconditions allow it to run
		if (CheckConditionsInState(Action->Preconditions, Parent->State))
		{
			//create new updated world state based on current state and action effects
			TMap<FString, bool> CurrentState = PopulateNewState(Parent->State, Action->Effects);

			GOAPNode* Node = new GOAPNode;
			Node->Parent = Parent;
			Node->RunningCost = Parent->RunningCost + Action->ActionCost;
			Node->State = CurrentState;
			Node->Action = Action;

			//add to main list of nodes
			AllNodes.Add(Node);

			//check conditions to see if match goal
			if (CheckConditionsInState(GoalState, CurrentState))
			{
				//when goal is found add to goalnodes and set to true
				GoalNodes.Add(Node);
				HasFoundGoal = true;
			} else
			{
				//create subset of avail actions without current state
				TSet<GOAPAction*> ActionSubset = CreateActionSubset(AvailableActions, Action);

				//call buildgraphrecursive until out of actions
				HasFoundGoal = BuildGraphRecursive(AllNodes, Node, GoalNodes, ActionSubset, GoalState);
			}
		}
	}

	//whether or not goal was found
	return HasFoundGoal;
}

TSet<GOAPAction*> GOAPPlanner::CreateActionSubset(const TSet<GOAPAction*>& AvailableActions, GOAPAction* RemoveAction)
{
	//create subset of actions without provided action 
	TSet<GOAPAction*> NewActionSet;

	for (auto Action : AvailableActions)
	{
		if (Action != RemoveAction)
		{
			NewActionSet.Add(Action);
		}
	}

	return NewActionSet;
}

bool GOAPPlanner::CheckConditionsInState(TMap<FString, bool>& Conditions, TMap<FString, bool>& State)
{
	//checks if first set of conditions exist in world state
	for (auto Condition : Conditions)
	{
		bool* CurrentStateCondition = State.Find(Condition.Key);

		if (CurrentStateCondition)
		{
			if (Condition.Value != *CurrentStateCondition)
			{
				return false;
			}
		} else
		{
			return false;
		}
	}

	return true;
}

TMap <FString, bool> GOAPPlanner::PopulateNewState(const TMap<FString, bool>& CurrentState, TMap<FString, bool>& Changes)
{
	//generates new state based on current state
	TMap<FString, bool> NewState = CurrentState;

	//changes based on updated changes (usually the action's effect)
	for (auto Pairs : Changes)
	{
		NewState.Add(Pairs.Key, Pairs.Value);
	}

	return NewState;
}