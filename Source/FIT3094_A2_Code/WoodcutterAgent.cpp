// Fill out your copyright notice in the Description page of Project Settings.


#include "WoodcutterAgent.h"
#include "UObject/ConstructorHelpers.h"
#include "FIT3094_A2_Code/GOAPPlanner.h"

// Sets default values
AWoodcutterAgent::AWoodcutterAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Wood Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(RootComponent);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>CubeMeshInstance(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	ConstructorHelpers::FObjectFinder<UMaterial> MaterialInstance(TEXT("/Game/StarterContent/Materials/WoodcutterMat.WoodcutterMat"));
	if (CubeMeshInstance.Succeeded())
	{
		VisibleComponent->SetStaticMesh(CubeMeshInstance.Object);
		VisibleComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	}

	NumWood = 0;
	health = maxHealth;
}

// Called when the game starts or when spawned
void AWoodcutterAgent::BeginPlay()
{
	Super::BeginPlay();

	//initialise actions
	CollectWoodAction* CollectWood = new CollectWoodAction();
	CollectWood->AddPrecondition("HasWood", false);
	CollectWood->AddEffect("HasWood", true);

	DropOffWoodAction* DropWood = new DropOffWoodAction();
	DropWood->AddPrecondition("HasWood", true);
	DropWood->AddEffect("HasWood", false);
	
	EatFoodAction* EatFood = new EatFoodAction();
	EatFood->AddPrecondition("IsHungry", true);
	EatFood->AddEffect("IsHungry", false);
	
	//add to available actions
	AvailableActions.Add(CollectWood);	
	AvailableActions.Add(DropWood);
	AvailableActions.Add(EatFood);

	//timer to decrease health
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AWoodcutterAgent::DecreaseHealth, 2.0f, true, 2.0f);

}

// Called every frame
void AWoodcutterAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TMap<FString, bool> AWoodcutterAgent::GetWorldState()
{
	TMap<FString, bool> WorldState = Super::GetWorldState();
	WorldState.Add("HasWood", NumWood == 50);
	WorldState.Add("IsHungry", health <= 15);

	return WorldState;
}

TMap<FString, bool> AWoodcutterAgent::CreateGoalState()
{
	TMap<FString, bool> GoalState;

	//set goal state depending on condition
	if (health <= 15)
	{
		GoalState.Add("IsHungry", false);
	} else
	{
		if (NumWood < 50)
		{
			GoalState.Add("HasWood", false);
		}

		else
		{
			GoalState.Add("HasWood", true);
		}
	}

	return GoalState;
}

void AWoodcutterAgent::DecreaseHealth()
{
	health--;

	if (health <= 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		Destroy();
	}
}