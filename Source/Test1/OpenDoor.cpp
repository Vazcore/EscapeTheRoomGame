// Alex Gaba, 2019

#include "OpenDoor.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Classes/GameFramework/PlayerController.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	Owner = GetOwner();
	DoorRotator = Owner->GetActorRotation();
}

void UOpenDoor::OpenDoor()
{
	DoorRotator.Yaw = OpenAngle;
	Owner->SetActorRotation(DoorRotator);
}

void UOpenDoor::CloseDoor()
{
	DoorRotator.Yaw = CloseAngle;
	Owner->SetActorRotation(DoorRotator);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CurrentTime = GetWorld()->GetTimeSeconds();

	// Polling aka asking if actor is in the volume to open the door
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor();
		LastTimeDoorWasOpened = CurrentTime;
	}
	else {
		if (CurrentTime >= LastTimeDoorWasOpened + CloseDoorTimeDelay) {
			CloseDoor();
		}
	}
}

