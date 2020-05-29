// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectBuildMatAction.h"
#include "FIT3094_A2_Code/BuilderAgent.h"
#include "GOAPActor.h"
#include "Kismet/KismetSystemLibrary.h"


CollectBuildMatAction::CollectBuildMatAction()
{
	Reset();
}

CollectBuildMatAction::~CollectBuildMatAction()
{

}

bool CollectBuildMatAction::IsActionDone()
{
	if (StoneGathered >= StoneToGather)
	{
		return true;
	}
	
	return false;
}

bool CollectBuildMatAction::CheckProcedurePrecondition(AGOAPActor* Agent)
{
	//if do village centre not set then find
	if (!VillageCentre)
	{
		//list of overlaps with other actors
		TArray<AActor*> mOverlaps;

		//actors to ignore
		TArray<AActor*> mIgnores;

		//object we're looking for
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

		UKismetSystemLibrary::SphereOverlapActors(Agent->GetWorld(), Agent->GetActorLocation(), 5000.0f, objectTypes, NULL, mIgnores, mOverlaps);

		//for every overlap that is found
		for (auto actor : mOverlaps)
		{
			AVillageCentreActor* villagePointer = Cast<AVillageCentreActor>(actor);
			//if successful then add to list
			if (villagePointer)
			{
				VillageCentre = villagePointer;
			}
		}
	}
	else {
		Target = VillageCentre;
		return true;
	}

	return false;
}


bool CollectBuildMatAction::PerformAction(AGOAPActor* Agent)
{
	AVillageCentreActor* VillageActor = Cast<AVillageCentreActor>(Target);
	ABuilderAgent* Builder = Cast<ABuilderAgent>(Agent);
	if (!VillageActor || !Builder)
	{
		return false;
	}

	if (Builder->health <= 15)
	{
		return false;
	}

	//time taken to gather materials
	if (FDateTime::UtcNow().ToUnixTimestamp() > TargetTime)
	{
		//if have space then gather material
		if (VillageActor->WoodResources > 0 && WoodGathered <= WoodToGather && Builder->NumWood < WoodToGather)
		{
			WoodGathered += 1;
			Builder->NumWood += 1;
			VillageActor->WoodResources -= 1;
		}
		
		if (VillageActor->StoneResources > 0 && StoneGathered <= StoneToGather && Builder->NumStone < StoneToGather) {
			StoneGathered += 1;
			Builder->NumStone += 1;
			VillageActor->StoneResources -= 1;
		}
		TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + CollectionTimer;
	}

	return true;
}

bool CollectBuildMatAction::RequiresInRange()
{
	return true;
}

void CollectBuildMatAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	StoneGathered = 0;
	WoodGathered = 0;
	TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + CollectionTimer;
}
