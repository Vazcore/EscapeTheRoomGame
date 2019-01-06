// Alex Gaba, 2019

#include "OpenDoor.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Classes/GameFramework/PlayerController.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	AActor* Owner = GetOwner();
	FRotator DoorRotator = Owner->GetActorRotation();
	DoorRotator.Yaw = OpenAngle;
	Owner->SetActorRotation(DoorRotator);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Polling aka asking if actor is in the volume to open the door
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		UE_LOG(LogTemp, Warning, TEXT("Opening the door"));
		OpenDoor();
	}
}

