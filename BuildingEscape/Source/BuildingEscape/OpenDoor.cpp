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

// Called every frame -- checks total mass on pressure plate. if mass is large enough then door will open
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float CurrentMass = GetTotalMassOfActorsOnPlate();

	// If there IS enough weight then open the door. DoorStatus prevents redundant event firings.
	if(CurrentMass > MassRequiredToOpenDoor && DoorIs(DoorStatus::Closed))
	{
		OnOpen.Broadcast();
		SetDoorStatus(DoorStatus::Open);
	}
	
	// If there IS NOT enough weight then close the door.  DoorStatus prevents redundant event firings.
	if (CurrentMass <= MassRequiredToOpenDoor && DoorIs(DoorStatus::Open))
	{
		OnClose.Broadcast();
		SetDoorStatus(DoorStatus::Closed);
	}

	// resync door status if out of sync
	// TODO Get actual position of the door and then check to see if this is a valid thing to do
	/*
	if (GetOwner()->GetActorRotation().Yaw == 0.0)
	{
		SetDoorStatus(DoorStatus::Closed);
	}
	*/
}

// returns true if door state is the same as the specified status
const bool UOpenDoor::DoorIs(const DoorStatus Status)
{
	return DoorState == Status;
}

// sets door state to the specified status
void UOpenDoor::SetDoorStatus(const DoorStatus Status)
{
	DoorState = Status;
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

