// Fill out your copyright notice in the Description page of Project Settings.

#include "DropOffWoodAction.h"
#include "FIT3094_A2_Code/WoodcutterAgent.h"
#include "GOAPActor.h"
#include "Kismet/KismetSystemLibrary.h"

DropOffWoodAction::DropOffWoodAction()
{
	ActionCost = 2;
	Reset();
}

DropOffWoodAction::~DropOffWoodAction()
{

}

bool DropOffWoodAction::IsActionDone()
{
	if (WoodDropped >= WoodToDrop)
	{
		return true;
	}
	
	return false;
}

bool DropOffWoodAction::CheckProcedurePrecondition(AGOAPActor* Agent)
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
		Target = VillageCentre;
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Wood Drop not found"));
	return false;
}


bool DropOffWoodAction::PerformAction(AGOAPActor* Agent)
{
	//checking if types are valid then do logic (drop off wood)
	AVillageCentreActor* VillageActor = Cast<AVillageCentreActor>(Target);
	AWoodcutterAgent* Woodcutter = Cast<AWoodcutterAgent>(Agent);
	if (!VillageActor || !Woodcutter)
	{
		return false;
	}

	if (Woodcutter->health <= 15)
	{
		return false;
	}

	if (FDateTime::UtcNow().ToUnixTimestamp() > TargetTime)
	{
		if (Woodcutter->NumWood > 0) {
			WoodDropped += 1;
			VillageActor->WoodResources += 1;
			Woodcutter->NumWood -= 1;
			TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + WoodDropTimer;
		}
	}

	return true;
}

bool DropOffWoodAction::RequiresInRange()
{
	return true;
}

void DropOffWoodAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	WoodDropped = 0;
	TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + WoodDropTimer;
}
