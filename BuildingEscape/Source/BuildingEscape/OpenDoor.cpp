// Copyright (c) 2018 - All Rights Reserved

#include "OpenDoor.h"


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

}

void UOpenDoor::OpenDoor(float AngleToOpen=60.0)
{
	// save open angle for UI display purposes (for now)
	OpenAngle = AngleToOpen;

	// get current owner
	AActor* Owner = GetOwner();

	// create a Rotator (Pitch,Yaw,Roll)
	FRotator NewRotation = FRotator(0.0, OpenAngle, 0.0);

	// set the door rotation
	Owner->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if actor is in trigger volume then open door
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(); // door opens to 60 degrees by default
	}
	else
	{
		OpenDoor(0.0); // close the door
	}
}

