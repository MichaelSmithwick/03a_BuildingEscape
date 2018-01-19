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


// Called when the game play starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	if (!PressurePlate)
	{
		UE_LOG(LogTemp,Error,TEXT("%s is missing PressurePlate."),*(GetOwner()->GetName()))
	}
}

// opens the door to UOpenDoor::OpenAngle degrees
void UOpenDoor::OpenDoor()
{
	GetOwner()->SetActorRotation(FRotator(0.0, OpenAngle, 0.0));
}

// closes the door
void UOpenDoor::CloseDoor()
{
	GetOwner()->SetActorRotation(FRotator(0.0, 0.0, 0.0));
}


// Called every frame -- checks total mass on pressure plate. if mass is large enough then door will open
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if enough weight is on pressure plate then door will open
	// and UOpenDoor::LastDoorOpenTime will be set to current game time
	if(GetTotalMassOfActorsOnPlate() > OpenMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// close door after time delay set by UOpenDoor::DoorCloseDelay
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}

}

// add up the weight of all AActor types that overlap with the pressure plate
float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	// if there is no PressurePlate then do nothing and exit
	if (!PressurePlate) { return 0.0; }

	float TotalMass = 0.0;

	// Find all the overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	
	// Iterate through them adding their masses
	for (const auto TheActor : OverlappingActors)
	{
		TotalMass += TheActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

// use this function to log information about an Actor that is on the presser plate
// @input TheActor - the actor on the plate
// @input TotalMass - total mass of all actors on the pressure plate
const void UOpenDoor::LogPressurePlateInformation(const AActor* TheActor, const float TotalMass)
{
	if (!TheActor) { return; }
	UE_LOG(LogTemp,
		Warning,
		TEXT("%s is on Pressure Plate at %f kg for a total of %f kg"),
		*(TheActor->GetName()),
		TheActor->FindComponentByClass<UPrimitiveComponent>()->GetMass(),
		TotalMass)
}

