// Noah González Sanz


#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp,Warning,TEXT("Grab action"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp,Warning,TEXT("Grab release action"));
}
// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// Checks Physics Handle Component
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(!PhysicsHandle)
	{
		UE_LOG(LogTemp,Error,TEXT("No se encuentra el Physics Handle Component dentro de %s"),*GetOwner()->GetName());
	}

	//Checks UInputComponenet
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComponent)
	{
		UE_LOG(LogTemp,Warning,TEXT("OK -> Se ha encontrado el componenet UInputComponent dentro de %s"),*GetOwner()->GetName());
	}else
	{
		UE_LOG(LogTemp,Error,TEXT("No se encuentra el UInputComponent dentro de %s"),*GetOwner()->GetName());
	}

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
	
	
}
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Obtener el Viewport de DefaultPawn_BP
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation,PlayerViewPointRotation);
	//Depuramos
	//UE_LOG(LogTemp,Warning,TEXT("La localizacion del viewport es: %s"),*PlayerViewPointLocation.ToString());
	//UE_LOG(LogTemp,Warning,TEXT("La rotacion del viewport es: %s"),*PlayerViewPointRotation.ToString());


	FVector LineTraceEnd;
	LineTraceEnd = PlayerViewPointLocation+PlayerViewPointRotation.Vector()*Reach;

	//Dibujamos la linea

	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor::Magenta,
		true,
		0.f,
		1,
		1.f
		);

	//Ray Tracing
	FHitResult Hit;

	FCollisionQueryParams TraceParams(
		FName(""),
		false,
		GetOwner());

	
	bool HasImpacted = GetWorld()->LineTraceSingleByObjectType(
		Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECC_PhysicsBody),
		TraceParams
		);

	if(HasImpacted)
	{
		UE_LOG(LogTemp,Warning,TEXT("Estas mirando %s"),*Hit.GetActor()->GetName());
	}
}

