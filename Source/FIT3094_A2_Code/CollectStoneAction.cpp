// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectStoneAction.h"
#include "FIT3094_A2_Code/StonemasonAgent.h"
#include "GOAPActor.h"
#include "Kismet/KismetSystemLibrary.h"


CollectStoneAction::CollectStoneAction()
{
	ActionCost = 2;
	Reset();
}

CollectStoneAction::~CollectStoneAction()
{

}

bool CollectStoneAction::IsActionDone()
{
	if (StoneGathered >= StoneToGather)
	{
		return true;
	}

	return false;
}

bool CollectStoneAction::CheckProcedurePrecondition(AGOAPActor* Agent)
{
	//if do not already have stone then find
	if (StoneList.Num() == 0)
	{
		// list of overlaps with other actors
		TArray<AActor*> mOverlaps;

		//actors to ignore
		TArray<AActor*> mIgnores;

		//object we're looking for
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

		UKismetSystemLibrary::SphereOverlapActors(Agent->GetWorld(), Agent->GetActorLocation(), 16000.0f, objectTypes, NULL, mIgnores, mOverlaps);

		//for every overlap that is found
		for (auto actor : mOverlaps)
		{
			//cast as stone
			AStoneActor* stonePointer = Cast<AStoneActor>(actor);
			//if successful, then add to list
			if (stonePointer)
			{
				StoneList.Add(stonePointer);
			}
		}
	}

	//find nearest stone

	//start with null pointer
	AStoneActor* NearestStone = nullptr;

	//for every tree in list above
	for (auto StoneActor : StoneList)
	{
		// if we already have a tree then check distance between current stone and nearest stone and if smaller then set new nearest stone
		if (NearestStone)
		{
			if (FVector::Dist(StoneActor->GetActorLocation(), Agent->GetActorLocation()) < FVector::Dist(NearestStone->GetActorLocation(), Agent->GetActorLocation()))
			{
				NearestStone = StoneActor;
			}
		}
		else
		{
			NearestStone = StoneActor;
		}
	}

	if (NearestStone)
	{
		Target = NearestStone;
		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Stone not found"));
	return false;
}


bool CollectStoneAction::PerformAction(AGOAPActor* Agent)
{
	AStoneActor* StoneActor = Cast<AStoneActor>(Target);
	AStonemasonAgent* Stonemason = Cast<AStonemasonAgent>(Agent);
	if (!StoneActor || !Stonemason)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stone not found"));
		return false;
	}

	if (Stonemason->health <= 15)
	{
		return false;
	}
	
	//time taken to gather stone
	if (FDateTime::UtcNow().ToUnixTimestamp() > TargetTime)
	{
		//if stone is available to be mined then gather
		if (StoneActor->StoneResources > 0 && Stonemason->NumStone < StoneToGather) {
			StoneGathered += 1;
			StoneActor->StoneResources -= 1;
			Stonemason->NumStone += 1;

			TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + StoneTimer;
		}
		//if it doesn't then return false 
		else
		{
			return false;
		}
	}

	return true;
}

bool CollectStoneAction::RequiresInRange()
{
	return true;
}

void CollectStoneAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	StoneGathered = 0;
	TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + StoneTimer;
}
