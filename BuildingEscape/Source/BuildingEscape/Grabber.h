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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	float Reach = 200.0;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* PawnInputComponent = nullptr;

	// handles user input
	void Grab();	// key down, button pushed, ...
	void Release();	// key up, button released, ...

	// find attached physics handle
	void FindPhysicsHandleComponent();

	// Log an error to the log Output Log
	const void LogError(const FString Name, const FString File, const FString Function, const int32 ErrorLine);

	// Find and Setup player input component
	void FindAndSetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult  GetFirstPhysicsBodyInReach();

	// Get vector location of the extent of the players reach
	const FVector GetPlayerReachEnd();

	// Get vector location of the base of the players reach
	const FVector GetPlayerReachBase();
};
