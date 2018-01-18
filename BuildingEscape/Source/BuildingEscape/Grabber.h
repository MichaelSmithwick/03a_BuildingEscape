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
	float Reach = 50.0;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* PawnInputComponent = nullptr;

	// handles user input
	void Grab();	// key down, button pushed, ...
	void Release();	// key up, button released, ...

	// find attached physics handle
	void FindPhysicsHandleComponent();

	// setup (assumed) user input component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult  GetFirstPhysicsBodyInReach();

	// Get vector location of the extent of the players reach
	const FVector GetPlayerReach(FVector& DefaultPawnLocation);
};
