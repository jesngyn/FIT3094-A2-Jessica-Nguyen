// Fill out your copyright notice in the Description page of Project Settings.


#include "VillageCentreActor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AVillageCentreActor::AVillageCentreActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	VisibleComponent->SetupAttachment(RootComponent);
	
	WoodResources = 0;
	StoneResources = 0;
}

// Called when the game starts or when spawned
void AVillageCentreActor::BeginPlay()
{
	Super::BeginPlay();
	VisibleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
}

// Called every frame
void AVillageCentreActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

