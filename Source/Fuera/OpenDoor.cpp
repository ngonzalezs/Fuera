// Noah González Sanz


#include "OpenDoor.h"

#include "NetworkMessage.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Windows/LiveCodingServer/Public/ILiveCodingServer.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	
	TargetYaw = InitialYaw + RotationYaw;
	
	DoorSound = GetOwner()->FindComponentByClass<UAudioComponent>();

	//PressurePlate = GetOwner()->FindComponentByClass<UBoxComponent>();
	
	DoorMesh = Cast<UMeshComponent>(GetOwner()->GetDefaultSubobjectByName("SM_Door"));
}

float UOpenDoor::TotalMassOfActorsInVolume() const 
{
	TArray<AActor*> OverlappingActors;
	if(PressurePlate)
	{
		PressurePlate->GetOverlappingActors(OverlappingActors);
	}
	
	float MassAcumulator = 0.0f;
	
	for (int i=0;i<OverlappingActors.Num();i++)
	{
		UPrimitiveComponent* ActorWithPrimitiveComponent = OverlappingActors[i]->FindComponentByClass<UPrimitiveComponent>();
		if(ActorWithPrimitiveComponent && ActorWithPrimitiveComponent->ComponentHasTag(*OpenerTag)){
			{
				MassAcumulator += OverlappingActors[i]->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			}
		}
	}
	//UE_LOG(LogTemp,Warning,TEXT("%f"),MassAcumulator);
	return MassAcumulator;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(PressurePlate && TotalMassOfActorsInVolume() > OpeningMass)	
	{
		OpenDoor(DeltaTime);
		InitialTimeOpening = GetWorld()->GetTimeSeconds();
	}else
	{
		if(GetWorld()->GetTimeSeconds()>(InitialTimeOpening+ClosingDelay))
		{
			CloseDoor(DeltaTime);
		}
		
	}
	
	//Pintamos la rotación actual y el Yaw:
	//UE_LOG(LogTemp,Warning,TEXT("%s"),*GetOwner()->GetActorRotation().ToString());
	//UE_LOG(LogTemp,Warning,TEXT("Yaw is %f"),GetOwner()->GetActorRotation().Yaw);
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	//Calculamos desde el Yaw actual hasta el objetivo, cuánto valdria el próximo paso (step) con un alpha de 0.01
	//Ojo, en cada paso estás mas cerca del objetivo, por eso cada vez se relentiza más la puerta.
	float nextStepYaw = FMath::FInterpTo(DoorMesh->GetComponentRotation().Yaw,TargetYaw,DeltaTime,OpenSpeed);

	//Creamos un Rotator con ese paso y lo aplicamos al actor:
	FRotator Rotation90Yaw(0.f,nextStepYaw,0.f);
	DoorMesh->SetWorldRotation(Rotation90Yaw);
	if(DoorSound && YaHeSonado == false)
	{
		DoorSound->SetSound(OpenSound);
		DoorSound->Play();
		YaHeSonado = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime){
	
	float nextStepYaw = FMath::FInterpTo(DoorMesh->GetComponentRotation().Yaw,InitialYaw,DeltaTime,6.0f);

	//Creamos un Rotator con ese paso y lo aplicamos al actor:
	FRotator Rotation90Yaw(0.f,nextStepYaw,0.f);
	DoorMesh->SetWorldRotation(Rotation90Yaw);

	if(DoorSound && YaHeSonado == true)
	{
		DoorSound->SetSound(CloseSound);
		DoorSound->Play();
		YaHeSonado = false;
	}
}



