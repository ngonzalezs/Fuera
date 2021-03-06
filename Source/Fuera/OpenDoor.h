// Noah González Sanz

#pragma once

#include "CoreMinimal.h"
#include "ActorFactories/ActorFactoryTriggerBox.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/TriggerVolume.h"
#include "UObject/Object.h"
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
	AActor* PressurePlate = nullptr;
	
	float InitialTimeOpening;
	
	UPROPERTY(EditAnywhere)
	float ClosingDelay = 2.0f;

	UPROPERTY(EditAnywhere)
	float OpeningMass = 10.0f;

	float TotalMassOfActorsInVolume() const;
	
	UPROPERTY()
	UAudioComponent* DoorSound = nullptr;
	
	bool YaHeSonado = false;

	UPROPERTY(EditAnywhere)
	USoundBase* OpenSound = nullptr;
	
	UPROPERTY(EditAnywhere)
	USoundBase* CloseSound = nullptr;

	UPROPERTY()
	UMeshComponent* DoorMesh = nullptr;

	FString OpenerTag = "DoorOpener";
	
	

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
};
