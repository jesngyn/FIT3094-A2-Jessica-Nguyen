// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectWoodAction.h"
#include "FIT3094_A2_Code/WoodcutterAgent.h"
#include "GOAPActor.h"
#include "Kismet/KismetSystemLibrary.h"

CollectWoodAction::CollectWoodAction()
{
	//initialise
	ActionCost = 2;
	Reset();
}

CollectWoodAction::~CollectWoodAction()
{
	
}

bool CollectWoodAction::IsActionDone()
{
	//check if action is completed by comparing number of wood gathered vs number of wood to gather
	if (WoodGathered >= WoodToGather)
	{
		return true;
	}

	return false;
}

bool CollectWoodAction::CheckProcedurePrecondition(AGOAPActor* Agent)
{
	// if do not already have wood then find
	if (TreeList.Num() == 0)
	{
		// list of overlaps with other actors
		TArray<AActor*> mOverlaps;

		// actors to ignore
		TArray<AActor*> mIgnores;

		//object we're looking for
		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

		//generate sphere overlap event with other actors using agent's pos and radius 
		UKismetSystemLibrary::SphereOverlapActors(Agent->GetWorld(), Agent->GetActorLocation(), 16000.0f, objectTypes, NULL, mIgnores, mOverlaps);

		//for every overlap that is found
		for (auto actor : mOverlaps)
		{
			//try to cast as a tree
			ATreeActor* woodPointer = Cast<ATreeActor>(actor);
			//if successful, then add to list
			if (woodPointer)
			{
				TreeList.Add(woodPointer);
			}
		}
	}

	//find nearest tree

	//start with null pointer
	ATreeActor* NearestTree = nullptr;

	//for every tree in the list found above
	for (auto TreeActor : TreeList)
	{
		// if we already have a tree then check distance between current tree and nearest tree and if smaller then set new nearest tree
		if (NearestTree)
		{
			if (FVector::Dist(TreeActor->GetActorLocation(), Agent->GetActorLocation()) < FVector::Dist(NearestTree->GetActorLocation(), Agent->GetActorLocation()))
			{
				NearestTree = TreeActor;
			}
		} else
		{
			NearestTree = TreeActor;
		}
	}

	//if nearest tree isn't null then set it as the target and return true
	if (NearestTree)
	{
		Target = NearestTree;
		return true;
	}

	//failed
	return false;
}


bool CollectWoodAction::PerformAction(AGOAPActor* Agent)
{
	//cast agent and target into their class types
	ATreeActor* TreeActor = Cast<ATreeActor>(Target);
	AWoodcutterAgent* Woodcutter = Cast<AWoodcutterAgent>(Agent);

	//if any of the casts are unsuccessful, return false
	if (!TreeActor || !Woodcutter)
	{
		return false;
	}

	if (Woodcutter->health <= 15)
	{
		return false;
	}
	
	//time taken to gather wood
	if (FDateTime::UtcNow().ToUnixTimestamp() > TargetTime)
	{
		//if tree has wood available then gather
		if (TreeActor->WoodResources > 0 && Woodcutter->NumWood < WoodToGather) {
			WoodGathered += 1;
			TreeActor->WoodResources -= 1;
			Woodcutter->NumWood += 1;

			TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + WoodTimer;
		}
		//if it doesn't then return false 
		else
		{
			return false;
		}
	}

	return true;
}

bool CollectWoodAction::RequiresInRange()
{
	return true;
}

void CollectWoodAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	WoodGathered = 0;
	TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + WoodTimer;
}
