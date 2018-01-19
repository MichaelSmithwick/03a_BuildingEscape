// Copyright (c) 2018 - All Rights Reserved

#include "OpenDoor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

// whitespace macro for marking function parameters that are changed
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	//ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();

}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.0, OpenAngle, 0.0));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0, 0.0, 0.0));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if actor is in trigger volume then open door
	//if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	if(GetTotalMassOfActorsOnPlate() > OpenMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	// Iterate through them adding their masses
	for (const auto TheActor : OverlappingActors)
	{
		TotalMass += TheActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp,
			Warning,
			TEXT("%s is on Pressure Plate at %f kg for a total of %f kg"),
			*(TheActor->GetName()),
			TheActor->FindComponentByClass<UPrimitiveComponent>()->GetMass(),
			TotalMass)
	}

	return TotalMass;
}

