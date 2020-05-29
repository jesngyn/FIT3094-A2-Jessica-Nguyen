// Fill out your copyright notice in the Description page of Project Settings.

#include "GOAPActor.h"
#include "FIT3094_A2_Code/GOAPPlanner.h"

// Sets default values
AGOAPActor::AGOAPActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoveSpeed = 600;

	ActionStateMachine = new StateMachine<Actor_States, AGOAPActor>(this, State_Nothing);
	ActionStateMachine->RegisterState(State_Idle, &AGOAPActor::OnIdleEnter, &AGOAPActor::OnIdleUpdate, &AGOAPActor::OnIdleExit);
	ActionStateMachine->RegisterState(State_Action, &AGOAPActor::OnActionEnter, &AGOAPActor::OnActionUpdate, &AGOAPActor::OnActionExit);
	ActionStateMachine->RegisterState(State_Move, &AGOAPActor::OnMoveEnter, &AGOAPActor::OnMoveUpdate, &AGOAPActor::OnMoveExit);
	ActionStateMachine->ChangeState(State_Idle);
}

// Called when the game starts or when spawned
void AGOAPActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGOAPActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ActionStateMachine->Tick(DeltaTime);
}


void AGOAPActor::OnIdleEnter()
{

}

void AGOAPActor::OnIdleUpdate(float DeltaTime)
{
	//plan next move
	//get world state
	TMap<FString, bool> WorldState = GetWorldState();
	//get goal state
	TMap<FString, bool> GoalState = CreateGoalState();

	//make plan and check success
	if (GOAPPlanner::Plan(this, AvailableActions, CurrentActions, WorldState, GoalState))
	{
		UE_LOG(LogTemp, Warning, TEXT("GOAP AGENT CLASS: Plan has been found"));
		ActionStateMachine->ChangeState(State_Action);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GOAP AGENT CLASS: No Plan Found!!!"));
	}
}

void AGOAPActor::OnIdleExit()
{

}

void AGOAPActor::OnMoveEnter()
{
	//entering move state to ensure we can move
	//if no actions, return to idle
	if (CurrentActions.IsEmpty())
	{
		ActionStateMachine->ChangeState(State_Idle);
		return;
	}

	//if current action needs an inrange check and target is null, plan again
	GOAPAction* CurrentAction = *CurrentActions.Peek();
	if (CurrentAction->RequiresInRange() && CurrentAction->Target == nullptr)
	{
		ActionStateMachine->ChangeState(State_Idle);
	}
}

void AGOAPActor::OnMoveUpdate(float DeltaTime)
{
	
	GOAPAction* CurrentAction = *CurrentActions.Peek();

	//move towards target based on action and set inrange to true when in range of the target actor
	FVector Direction = CurrentAction->Target->GetActorLocation() - GetActorLocation();
	Direction.Normalize();

	//update based on direction & move speed
	FVector NewPos = GetActorLocation() + Direction * MoveSpeed * DeltaTime;

	//if close enough to position then snap to it
	if (FVector::Dist(NewPos, CurrentAction->Target->GetActorLocation()) <= Tolerance)
	{
		NewPos = CurrentAction->Target->GetActorLocation();
		//now in range so set action inrange to true
		CurrentAction->SetInRange(true);
		//move onto action state
		ActionStateMachine->ChangeState(State_Action);
	}

	SetActorLocation(NewPos);
}

void AGOAPActor::OnMoveExit()
{

}

void AGOAPActor::OnActionEnter()
{

}

void AGOAPActor::OnActionUpdate(float DeltaTime)
{
	//if no states change to idle
	if (CurrentActions.IsEmpty())
	{
		ActionStateMachine->ChangeState(State_Idle);
		return;
	}

	//check if current action is completed
	GOAPAction* CurrentAction = *CurrentActions.Peek();
	if (CurrentAction->IsActionDone())
	{
		//pop from queue
		CurrentActions.Dequeue(CurrentAction);
	}

	//if more actions then continue
	if (!CurrentActions.IsEmpty())
	{
		//get top of queue
		CurrentAction = *CurrentActions.Peek();

		//check if we need to be in range for next action, if no then return true, if yes then check if in range
		bool InRange = CurrentAction->RequiresInRange() ? CurrentAction->IsInRange() : true;

		//if in range perform action
		if (InRange)
		{
			//if action fails, store result
			bool IsActionSuccessful = CurrentAction->PerformAction(this);
			UE_LOG(LogTemp, Warning, TEXT("GOAP AGENT CLASS: Action Performed"));

			if (!IsActionSuccessful)
			{
				OnPlanAborted(CurrentAction);
			}
		}
		else
		{
			//move to action if not in range
			ActionStateMachine->ChangeState(State_Move);
		}
	}
	else
	{
		//no actions remaining - change to idle state
		ActionStateMachine->ChangeState(State_Idle);
	}
}

void AGOAPActor::OnActionExit()
{

}

TMap<FString, bool> AGOAPActor::GetWorldState()
{
	TMap<FString, bool> CurrentWorldState;
	return CurrentWorldState;
}

TMap<FString, bool> AGOAPActor::CreateGoalState()
{
	TMap<FString, bool> GoalState;
	return GoalState;
}

void AGOAPActor::OnPlanFailed(TMap<FString, bool> FailedGoalState)
{

}

void AGOAPActor::OnPlanAborted(GOAPAction* FailedAction)
{

}