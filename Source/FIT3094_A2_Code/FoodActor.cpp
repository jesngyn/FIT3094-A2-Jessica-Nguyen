// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodActor.h"

#include "FIT3094_A2_Code/BuilderAgent.h"
#include "FIT3094_A2_Code/WoodcutterAgent.h"
#include "FIT3094_A2_Code/StonemasonAgent.h"

// Sets default values
AFoodActor::AFoodActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visible Component"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));
	VisibleComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetupAttachment(VisibleComponent);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFoodActor::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AFoodActor::BeginPlay()
{
	Super::BeginPlay();
	VisibleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
}

// Called every frame
void AFoodActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFoodActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//ensure other actor is a goap actor
	if (AGOAPActor* Actor = Cast<AGOAPActor>(OtherActor)) {
		
		//check what type other actor is and give health, then destroy
		if (AWoodcutterAgent* WoodCutter = Cast<AWoodcutterAgent>(OtherActor))
		{
			WoodCutter->health = WoodCutter->maxHealth;
		}

		else if (ABuilderAgent* Builder = Cast<ABuilderAgent>(OtherActor))
		{
			Builder->health = Builder->maxHealth;
		}

		else if (AStonemasonAgent* Stonemason = Cast<AStonemasonAgent>(OtherActor))
		{
			Stonemason->health = Stonemason->maxHealth;
		}

		Destroy();
	}
}

