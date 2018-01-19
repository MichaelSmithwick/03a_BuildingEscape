// Copyright (c) 2018 - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// called to open/close the door
	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	// door open angle
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = 90.0;

	// The PressurePlate is assigned externally by user/designer in UE4 and is persistent from run to run
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	// The delay before the door is closed after leaving pressure pad
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.5;

	// the game time the door was last opened
	float LastDoorOpenTime;

	// minimum mass needed on pressure pad to open door
	UPROPERTY(EditAnywhere)
	int32 OpenMass = 50.0;
	
	// get sum of all mass on the pressure plate
	float GetTotalMassOfActorsOnPlate();

	// log pressure plate item weight and total weight
	const void LogPressurePlateInformation(const AActor* TheActor, const float TotalMass);
};
