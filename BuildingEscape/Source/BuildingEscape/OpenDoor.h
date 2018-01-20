// Copyright (c) 2018 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

enum class DoorStatus {
	Closed=0,
	Open=1
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Door Open Event broadcast to Blueprint engine
	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnOpen;

	// Door Close Event broadcast to Blueprint engine
	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnClose;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// The PressurePlate is assigned externally by user/designer in UE4 and is persistent from run to run
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	bool bOpenClockwise = true;

	// Minimum mass in kg needed on pressure pad to open door
	UPROPERTY(EditAnywhere)
	int32 MassRequiredToOpenDoor = 50.0;

	// last known Door Status
	DoorStatus DoorState = DoorStatus::Closed;

	// returns true if door state is the same as the specified status
	const bool DoorIs(const DoorStatus Status);

	// sets door state to the specified status
	void SetDoorStatus(const DoorStatus Status);
	
	// get sum of all mass on the pressure plate in kg
	float GetTotalMassOfActorsOnPlate();

	// log pressure plate item weight and total weight
	const void LogPressurePlateInformation(const AActor* TheActor, const float TotalMass);
};
