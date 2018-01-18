// Copyright (c) 2018 - All Rights Reserved

#include "Grabber.h"
#include "Engine/World.h"
#include "Public/DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

// blank macro used to document changed parameters returned from functions
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.
	// You can turn these features off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;	// this must be true for TickComponent() function to be called.
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	SetupInputComponent();

}

// setup input component -- only appears at run time
void UGrabber::SetupInputComponent()
{
	PawnInputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (PawnInputComponent)
	{
		// bind input actions to input component and callback functions
		PawnInputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		PawnInputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);

		UE_LOG(LogTemp, Warning, TEXT("Found PawnInputComponent: %s"), *(PawnInputComponent->GetName()))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to get PawnInputComponent from %s."), *(GetOwner()->GetName()))
	}
}

// look for the PhysicsHandle
void UGrabber::FindPhysicsHandleComponent()
{
	// if 'PhysicsHandle' is not valid the retrieving line number will be returned with the error information
	int32 ErrorLine = __LINE__ + 1;
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// found - do nothing for now
	}
	else
	{
		// not found - report error
		// also check that Physics Handle component was added to the default pawn (i.e. player)
		FString FileName = FString(TEXT(__FILE__));
		FString FunctionName = FString(TEXT(__FUNCTION__));
		UE_LOG(LogTemp, Error, TEXT("%s DOES NOT HAVE A Physics Handle Component."), *(GetOwner()->GetName()))
		UE_LOG(LogTemp, Error, TEXT("Does %s have a Blueprint assigned Physics Handle object?"), *(GetOwner()->GetName()))
		UE_LOG(LogTemp, Error, TEXT("[ File: %s  Function: %s  Line Number: %d ]"), *FString(TEXT(__FILE__)), *FString(TEXT(__FUNCTION__)), ErrorLine)
	}
}

// handle user input -- input active, i.e. Key down, Button pushed
void UGrabber::Grab()
{
	// LINE TRACE and see if we reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// If we hit something then attach a physics handle
	if (ActorHit)
	{
		// TODO attach physics handle
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);

		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle grabs component."))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle DID NOT GRAB component."))
	}
}

// handle user input -- input released, i.e. Key up, Button release
void UGrabber::Release()
{
	// TODO release physics handle
	PhysicsHandle->ReleaseComponent();

	UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle Releases Component"))
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector DefaultPawnLocation;
	FVector LineTraceEnd = GetPlayerReach(DefaultPawnLocation);

	// setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;

	// Line-trace (aka ray-cast) out to 'Reach' distance
	bool IsHit = GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,				// detected object to be returned
		DefaultPawnLocation,	// where the pawn is
		LineTraceEnd,			// the end of the line trace
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), // what to look for
		TraceParameters			// don't detect default pawn, don't detect multiple objects
	);

	// Create a debug vector (pointing line) to show where the default pawn is looking
	DrawDebugLine(GetWorld(), DefaultPawnLocation, LineTraceEnd, FColor(255, 0, 0, 255), false, 0.0, 0, 10.0);

	// determine what was hit by 'line-trace'
	if (IsHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is hit."), *(Hit.GetActor()->GetName()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hit."));
	}

	return Hit;
}

const FVector UGrabber::GetPlayerReach(FVector& DefaultPawnLocation)
{
	// where the default pawn is at and the direction it is facing in
	FRotator DefaultPawnRotation;

	// get location and facing direction of default pawn
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(DefaultPawnLocation, DefaultPawnRotation);

	return DefaultPawnLocation + (DefaultPawnRotation.Vector() * Reach);
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector DefaultPawnLocation;
	FVector LineTraceEnd = GetPlayerReach(DefaultPawnLocation);

	// if physics handle is attached 
	if (PhysicsHandle->GrabbedComponent)
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

