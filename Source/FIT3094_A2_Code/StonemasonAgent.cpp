// Fill out your copyright notice in the Description page of Project Settings.


#include "StonemasonAgent.h"
#include "FIT3094_A2_Code/GOAPPlanner.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AStonemasonAgent::AStonemasonAgent() : AGOAPActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Stone"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(RootComponent);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshInstance(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	ConstructorHelpers::FObjectFinder<UMaterial> MaterialInstance(TEXT("/Game/StarterContent/Materials/StonemasonMat.StonemasonMat"));
	if (CubeMeshInstance.Succeeded())
	{
		VisibleComponent->SetStaticMesh(CubeMeshInstance.Object);
		VisibleComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	}
	NumStone = 0;
	health = maxHealth;
}

// Called when the game starts or when spawned
void AStonemasonAgent::BeginPlay()
{
	Super::BeginPlay();

	//initialise actions
	CollectStoneAction* CollectStone = new CollectStoneAction();
	CollectStone->AddPrecondition("HasStone", false);
	CollectStone->AddEffect("HasStone", true);

	DropOffStoneAction* DropStone = new DropOffStoneAction();
	DropStone->AddPrecondition("HasStone", true);
	DropStone->AddEffect("HasStone", false);

	
	EatFoodAction* EatFood = new EatFoodAction();
	EatFood->AddPrecondition("IsHungry", true);
	EatFood->AddEffect("IsHungry", false);
	

	//add to available actions
	AvailableActions.Add(CollectStone);
	AvailableActions.Add(DropStone);
	AvailableActions.Add(EatFood);

	//timer to decrease health
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AStonemasonAgent::DecreaseHealth, 2.0f, true, 2.0f);

}

// Called every frame
void AStonemasonAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TMap<FString, bool> AStonemasonAgent::GetWorldState()
{
	TMap<FString, bool> WorldState = Super::GetWorldState();
	WorldState.Add("HasStone", NumStone == 20);
	WorldState.Add("IsHungry", health <= 15);

	return WorldState;
}

TMap<FString, bool> AStonemasonAgent::CreateGoalState()
{
	TMap<FString, bool> GoalState;
	//set goal state depending on condition
	if (health <= 15)
	{
		GoalState.Add("IsHungry", false);
	}
	else
	{
		if (NumStone < 50)
		{
			GoalState.Add("HasStone", false);
		}

		else
		{
			GoalState.Add("HasStone", true);
		}
	}

	return GoalState;
}

void AStonemasonAgent::DecreaseHealth()
{
	health--;

	if (health <= 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		Destroy();
	}
}