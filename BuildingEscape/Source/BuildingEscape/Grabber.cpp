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
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Find the UPhysicsHandle and UInputComponent components of the Owner object
	FindPhysicsHandleComponent();
	FindAndSetupInputComponent();

}

// Find the Input Component and assign handling functions
// (Input Component only appears at run time)
// Action Name ("Grab") and key assignments are found in 'Project Settings' under 'Input' 
void UGrabber::FindAndSetupInputComponent()
{
	PawnInputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (PawnInputComponent)
	{
		PawnInputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		PawnInputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have an Input Component."), *(GetOwner()->GetName()))
	}
}

// find a PhysicsHandle component owned by this object's owner and store it in 'UGrabber::PhysicsHandle'
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		LogError(GetOwner()->GetName(),FString(TEXT(__FILE__)),FString(TEXT(__FUNCTION__)),__LINE__);
	}
}

// display error message to Output Log
// @input Object Name		-- Name of object (or any string)
// @input File Name			-- name of the file, usually the __FILE__ macro
// @input Function Name		-- name of the function, usually the __FUNCTION__ macro
// @input Line Number		-- line number of error, usually the __LINE__ macro
const void UGrabber::LogError(const FString Name, const FString File, const FString Function, const int32 ErrorLine)
{
	UE_LOG(LogTemp, Error, TEXT("%s DOES NOT HAVE A Physics Handle Component."), *Name)
	UE_LOG(LogTemp, Error, TEXT("Does %s have a Blueprint assigned Physics Handle object?"), *Name)
	UE_LOG(LogTemp, Error, TEXT("[ File: %s  Function: %s  Line Number: %d ]"), *File, *Function, ErrorLine)
}

// handle user active input, e.g. Key down, Button pushed
void UGrabber::Grab()
{
	// No PhysicsHandle then do nothing and leave
	if (!PhysicsHandle) { return; }

	// LINE TRACE and see if we reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();

	// If we hit something then attach a physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);

	}
}

// handle user released input, e.g. Key up, Button release
void UGrabber::Release()
{
	if (PhysicsHandle)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

// Create a debug vector (pointing line) to show where the default pawn is looking if needed
//DrawDebugLine(GetWorld(), GetPlayerReachBase(), GetPlayerReachEnd(), FColor(255, 0, 0, 255), false, 0.0, 0, 10.0);

// Ask World if there is a physics body in reach, if so, get it, and return it
// @return FHitResult .. valid only if found...
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// setup query parameters, Tag=="", Complex==false, Ignore==Object Owner
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult HitObject;

	// Test to see if there is an object in reach
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitObject,														// if detected the object to be returned
		GetPlayerReachBase(),												// pawn trace line base
		GetPlayerReachEnd(),												// end of reach
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),	// type of object to test for
		TraceParameters														// don't detect default pawn, don't detect multiple objects
	);

	return HitObject;
}

// Get the end location vector for the pawns reaching viewpoint
const FVector UGrabber::GetPlayerReachEnd()
{
	FRotator DefaultPawnRotation;
	FVector DefaultPawnLocation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(DefaultPawnLocation, DefaultPawnRotation);

	// UGrabber::Reach scales the unit vector returned by FRotator
	return DefaultPawnLocation + (DefaultPawnRotation.Vector() * Reach);
}

// Get the base location vector for the pawns reaching viewpoint
const FVector UGrabber::GetPlayerReachBase()
{
	FRotator DefaultPawnRotation;
	FVector DefaultPawnLocation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(DefaultPawnLocation, DefaultPawnRotation);

	return DefaultPawnLocation;
}

// Called every frame and moves held item
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if no PhysicsHandle then do nothing and leave
	if (!PhysicsHandle) { return; }

	// move held item to end of players reach line
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayerReachEnd());
	}

}

