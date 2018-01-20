// Copyright (c) 2018 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Controls how far Owner can reach out to grab another Actor
	UPROPERTY(EditAnywhere)
	float Reach = 200.0;

	// PhysicsHandle used to grab other Actors
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	// Gets player input
	UInputComponent* PawnInputComponent = nullptr;

	// handles user input
	void Grab();	// key down, button pushed, ...
	void Release();	// key up, button released, ...

	// find attached physics handle
	void FindPhysicsHandleComponent();

	// Find and Setup player input component
	void FindAndSetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult  GetFirstPhysicsBodyInReach();

	// Get vector location of the extent of the players reach
	const FVector GetPlayerReachEnd();

	// Get vector location of the base of the players reach
	const FVector GetPlayerReachBase();

	// Log an error to the log Output Log
	const void LogError(const FString Name, const FString File, const FString Function, const int32 ErrorLine);
};
