// Noah González Sanz


#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::Grab()
{
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	if(HitResult.GetActor())
	{
		if(PhysicsHandle)	
		{
			PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			FName (""),
			GetPlayersReach());
		}
	}
	
	UE_LOG(LogTemp,Warning,TEXT("Grab action"));
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit; //Donde se almacena si chocamos contra el Raycast
	FCollisionQueryParams TraceParams (FName(TEXT("")),false,GetOwner());
	
	//Comprobar que alcanzamos el Raycast
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);
	
	return Hit;
}

void UGrabber::Release()
{
	if(PhysicsHandle)
	{
		PhysicsHandle->ReleaseComponent();
	}
	UE_LOG(LogTemp,Warning,TEXT("Grab release action"));
}

FVector UGrabber::GetPlayerWorldPos() const
{
	//Obtener el Viewport de DefaultPawn_BP
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation,OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}



FVector UGrabber::GetPlayersReach() const
{
	//Obtener el Viewport de DefaultPawn_BP
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation,OUT PlayerViewPointRotation);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
}
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if(PhysicsHandle && PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
	//Dibujamos la linea
	/*DrawDebugLine(GetWorld(),PlayerViewPointLocation,LineTraceEnd,FColor::Magenta,true,0.f,1,1.f);*/
}

void UGrabber::FindPhysicsHandle()
{
	// Checks Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandle)
	{
		UE_LOG(LogTemp,Error,TEXT("No se encuentra el Physics Handle Component dentro de %s"),*GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	//Checks UInputComponenet
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		//Input 
		InputComponent->BindAction(
			"Grab",
			IE_Pressed,
			this,
			&UGrabber::Grab
			);

		InputComponent->BindAction(
			"Grab",
			IE_Released,
			this,
			&UGrabber::Release
		);
		UE_LOG(LogTemp,Warning,TEXT("OK -> Se ha encontrado el componenet UInputComponent dentro de %s"),*GetOwner()->GetName());
	}else
	{
		UE_LOG(LogTemp,Error,TEXT("No se encuentra el UInputComponent dentro de %s"),*GetOwner()->GetName());
	}
}



