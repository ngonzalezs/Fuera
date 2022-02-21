// Noah González Sanz


#include "OpenDoor.h"

#include "NetworkMessage.h"
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
	TargetYaw = GetOwner()->GetActorRotation().Yaw + RotationYaw;
	
	if(!ActorThatOpensDoor)
	{
		ActorThatOpensDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(PressurePlate->IsOverlappingActor(ActorThatOpensDoor))
	{
		OpenDoor(DeltaTime);
		InitialTimeOpening = GetWorld()->GetTimeSeconds();
	}else
	{
		if(GetWorld()->GetTimeSeconds()>InitialTimeOpening+ClosingDelay)
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
	float nextStepYaw = FMath::FInterpTo(GetOwner()->GetActorRotation().Yaw,TargetYaw,DeltaTime,OpenSpeed);

	//Creamos un Rotator con ese paso y lo aplicamos al actor:
	FRotator Rotation90Yaw(0.f,nextStepYaw,0.f);
	GetOwner()->SetActorRotation(Rotation90Yaw);
}

void UOpenDoor::CloseDoor(float DeltaTime){
	
	float nextStepYaw = FMath::FInterpTo(GetOwner()->GetActorRotation().Yaw,InitialYaw,DeltaTime,2.0f);

	//Creamos un Rotator con ese paso y lo aplicamos al actor:
	FRotator Rotation90Yaw(0.f,nextStepYaw,0.f);
	GetOwner()->SetActorRotation(Rotation90Yaw);
}



