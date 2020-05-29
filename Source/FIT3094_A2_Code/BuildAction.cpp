// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildAction.h"
#include "FIT3094_A2_Code/BuilderAgent.h"
#include "GOAPActor.h"
#include "Kismet/KismetSystemLibrary.h"

BuildAction::BuildAction()
{
	Reset();
}

BuildAction::~BuildAction()
{

}

bool BuildAction::IsActionDone()
{
	if (BuildWood >= WoodToBuild && BuildStone >= StoneToBuild)
	{
		return true;
	}

	return false;
}

bool BuildAction::CheckProcedurePrecondition(AGOAPActor* Agent)
{
	//check if building actor is set if not then find it
	if (!BuildingActor)
	{
		TArray<AActor*> mOverlaps;

		TArray<AActor*> mIgnores;

		TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
		objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

		UKismetSystemLibrary::SphereOverlapActors(Agent->GetWorld(), Agent->GetActorLocation(), 5000.0f, objectTypes, NULL, mIgnores, mOverlaps);

		for (auto actor : mOverlaps)
		{
			ABuildingActor* buildingPointer = Cast<ABuildingActor>(actor);
			if (buildingPointer)
			{
				BuildingActor = buildingPointer;
			}
		}
	}

	else {
		Target = BuildingActor;
		return true;
	}

	return false;
}


bool BuildAction::PerformAction(AGOAPActor* Agent)
{
	ABuildingActor* Building = Cast<ABuildingActor>(Target);
	ABuilderAgent* Builder = Cast<ABuilderAgent>(Agent);
	if (!Building || !Builder)
	{
		return false;
	}

	if (Builder->health <= 15)
	{
		return false;
	}
	
	//check resources required and drop them off if still required
	if (FDateTime::UtcNow().ToUnixTimestamp() > TargetTime)
	{
		if (BuildWood <= WoodToBuild && Builder->NumWood != 0) {
			BuildWood += 1;
			Builder->NumWood -= 1;
			Building->WoodResources += 1;
		}
		
		if (BuildStone <= StoneToBuild && Builder->NumStone != 0) {
			BuildStone += 1;
			Builder->NumStone -= 1;
			Building->StoneResources += 1;
		}
		
		TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + BuildTimer;
	}

	return true;
}

bool BuildAction::RequiresInRange()
{
	return true;
}

void BuildAction::Reset()
{
	SetInRange(false);
	Target = nullptr;
	BuildStone = 0;
	BuildWood = 0;
	TargetTime = FDateTime::UtcNow().ToUnixTimestamp() + BuildTimer;
}
