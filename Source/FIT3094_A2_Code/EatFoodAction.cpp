// Fill out your copyright notice in the Description page of Project Settings.

#include "EatFoodAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FIT3094_A2_Code/GOAPActor.h"
#include "FIT3094_A2_Code/BuilderAgent.h"
#include "FIT3094_A2_Code/WoodcutterAgent.h"
#include "FIT3094_A2_Code/StonemasonAgent.h"


EatFoodAction::EatFoodAction()
{
	ActionCost = 1;
	Reset();
}

EatFoodAction::~EatFoodAction()
{

}

bool EatFoodAction::IsActionDone()
{
	//check if action is completed
	if (FoodGathered >= FoodToGather)
	{
		return true;
	}

	return false;
}

bool EatFoodAction::CheckProcedurePrecondition(AGOAPActor* Agent)
{
	//if do not already have food then find
	if (FoodList.Num() == 0)
	{
		TArray<AActor*> mOverlaps;

		TArray<AActor*> mIgnores;

		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

		//generate sphere overlap event with other actors using agent's pos and radius 
		UKismetSystemLibrary::SphereOverlapActors(Agent->GetWorld(), Agent->GetActorLocation(), 16000.0f, objectTypes, NULL, mIgnores, mOverlaps);

		//for every overlap that is found
		for (auto actor : mOverlaps)
		{
			//try to cast as food actor
			AFoodActor* foodPointer = Cast<AFoodActor>(actor);
			if (foodPointer)
			{
				//if successful add to list
				FoodList.Add(foodPointer);
			}
		}
	}

	//finding nearest food actor
	AFoodActor* NearestFood = nullptr;

	//for every food in the list found above
	for (auto FoodActor : FoodList)
	{
		// if we already have a food then check distance between current food and nearest food and if smaller then set new nearest food
		if (NearestFood)
		{
			if (FVector::Dist(FoodActor->GetActorLocation(), Agent->GetActorLocation()) < FVector::Dist(NearestFood->GetActorLocation(), Agent->GetActorLocation()))
			{
				NearestFood = FoodActor;
			}
		}
		else
		{
			NearestFood = FoodActor;
		}
	}
	//if nearest food isnt null then set it as the target and return true
	if (NearestFood)
	{
		Target = NearestFood;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Food not found"));
	return false;
}


bool EatFoodAction::PerformAction(AGOAPActor* Agent)
{
	//cast agent and target into their class types
	AFoodActor* FoodActor = Cast<AFoodActor>(Target);
	AGOAPActor* Actor = Cast<AGOAPActor>(Agent);
	//if any of the casts are unsuccessful, return false
	if (!FoodActor || !Actor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Food not found"));
		return false;
	}
	
	//time taken to eat
	if (FDateTime::UtcNow().ToUnixTimestamp() > TargetTime)
	{
		FoodGathered += 1;
		FoodActor->Destroy();
		FoodList.Empty();
		TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + FoodTimer;
	}

	return true;
}

bool EatFoodAction::RequiresInRange()
{
	return true;
}

void EatFoodAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	FoodGathered = 0;
	TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + FoodTimer;
	FoodList.Empty();
}
