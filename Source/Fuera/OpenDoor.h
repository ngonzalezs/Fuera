// Noah González Sanz

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FUERA_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()
private :
	UPROPERTY(EditAnywhere)	
	float RotationYaw = 90.0f;
	float TargetYaw;
	float InitialYaw;
	
	UPROPERTY(EditAnywhere)	
	float OpenSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
//Se puede cargar
	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpensDoor= nullptr;
	
	float InitialTimeOpening;
	
	UPROPERTY(EditAnywhere)
	float ClosingDelay = 2.0f;

	UPROPERTY(EditAnywhere)
	float OpeningMass = 50.0f;

	float TotalMassOfActorsInVolume() const;

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
