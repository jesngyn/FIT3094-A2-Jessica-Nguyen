// Fill out your copyright notice in the Description page of Project Settings.

#include "DropOffStoneAction.h"
#include "FIT3094_A2_Code/StonemasonAgent.h"
#include "GOAPActor.h"
#include "Kismet/KismetSystemLibrary.h"

DropOffStoneAction::DropOffStoneAction()
{
	ActionCost = 2;
	Reset();
}

DropOffStoneAction::~DropOffStoneAction()
{

}

bool DropOffStoneAction::IsActionDone()
{
	if (StoneDropped >= StoneToDrop)
	{
		return true;
	}

	return false;
}

bool DropOffStoneAction::CheckProcedurePrecondition(AGOAPActor* Agent)
{
	//if village centre isn't valid loop through all actors and cast village centre against them until match
	if (!VillageCentre)
	{
		TArray<AActor*> mOverlaps;

		TArray<AActor*> mIgnores;

		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

		UKismetSystemLibrary::SphereOverlapActors(Agent->GetWorld(), Agent->GetActorLocation(), 16000.0f, objectTypes, NULL, mIgnores, mOverlaps);

		for (auto actor : mOverlaps)
		{
			AVillageCentreActor* villagePointer = Cast<AVillageCentreActor>(actor);
			if (villagePointer)
			{
				VillageCentre = villagePointer;
				Target = VillageCentre;
			}
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Stone Drop not found"));
		Target = VillageCentre;
		return true;
	}
	return false;
}

bool DropOffStoneAction::PerformAction(AGOAPActor* Agent)
{
	//cast agent and target into their class types
	AVillageCentreActor* VillageActor = Cast<AVillageCentreActor>(Target);
	AStonemasonAgent* Stonemason = Cast<AStonemasonAgent>(Agent);

	//if any of the casts are unsuccessful, return false
	if (!VillageActor || !Stonemason)
	{
		return false;
	}

	if (Stonemason->health <= 15)
	{
		return false;
	}
	
	//time taken to drop off
	if (FDateTime::UtcNow().ToUnixTimestamp() > TargetTime)
	{
		if (Stonemason->NumStone > 0) {
			StoneDropped += 1;
			VillageActor->StoneResources += 1;
			Stonemason->NumStone -= 1;
			TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + StoneDropTimer;
		}
	}

	return true;
}

bool DropOffStoneAction::RequiresInRange()
{
	return true;
}

void DropOffStoneAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	StoneDropped = 0;
	TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + StoneDropTimer;
}
