// Fill out your copyright notice in the Description page of Project Settings.


#include "BuilderAgent.h"
#include "UObject/ConstructorHelpers.h"
#include "FIT3094_A2_Code/GOAPPlanner.h"

// Sets default values
ABuilderAgent::ABuilderAgent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	VisibleComponent->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshInstance(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	ConstructorHelpers::FObjectFinder<UMaterial> MaterialInstance(TEXT("/Game/StarterContent/Materials/BuilderMat.BuilderMat"));
	if (CubeMeshInstance.Succeeded())
	{
		VisibleComponent->SetStaticMesh(CubeMeshInstance.Object);
		VisibleComponent->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	}

	NumWood = 0;
	NumStone = 0;
	health = maxHealth;
}

// Called when the game starts or when spawned
void ABuilderAgent::BeginPlay()
{
	Super::BeginPlay();
	
	//initialise actions
	CollectBuildMatAction* CollectMats = new CollectBuildMatAction();
	CollectMats->AddPrecondition("HasMats", false);
	CollectMats->AddEffect("HasMats", true);

	BuildAction* Build = new BuildAction();
	Build->AddPrecondition("HasMats", true);
	Build->AddEffect("HasMats", false);

	EatFoodAction* EatFood = new EatFoodAction();
	EatFood->AddPrecondition("IsHungry", true);
	EatFood->AddEffect("IsHungry", false);

	//add to available actions
	AvailableActions.Add(CollectMats);
	AvailableActions.Add(Build);
	AvailableActions.Add(EatFood);

	//timer to decrease health
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABuilderAgent::DecreaseHealth, 2.0f, true, 2.0f);
}

// Called every frame
void ABuilderAgent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TMap<FString, bool> ABuilderAgent::GetWorldState()
{
	TMap<FString, bool> WorldState = Super::GetWorldState();
	WorldState.Add("HasMats", NumStone == 5 && NumWood == 10);

	return WorldState;
}

TMap<FString, bool> ABuilderAgent::CreateGoalState()
{
	TMap<FString, bool> GoalState;

	if (health <= 15)
	{
		GoalState.Add("IsHungry", false);
	}
	else
	{
		if (NumStone < 5 || NumWood < 10)
		{
			GoalState.Add("HasMats", false);
		}

		else
		{
			GoalState.Add("HasMats", true);
		}
	}

	return GoalState;
}

void ABuilderAgent::DecreaseHealth()
{
	health--;

	if (health <= 0)
	{
		GetWorldTimerManager().ClearTimer(TimerHandle);
		Destroy();
	}
}