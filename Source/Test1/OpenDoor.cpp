// Alex Gaba, 2019

#include "OpenDoor.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Classes/GameFramework/PlayerController.h"

#define OUT


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
	OnOpen.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	OnClose.Broadcast();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOnThePlate() > MassRequiredToOpenDoor) {
		OpenDoor();
	}
	else {
		CloseDoor();
	}
}

float UOpenDoor::TotalMassOnThePlate() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;

	if (PressurePlate == nullptr) return TotalMass;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for (auto& Actor : OverlappingActors) 
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	UE_LOG(LogTemp, Warning, TEXT("Total mass is %s"), *FString::SanitizeFloat(TotalMass));
	return TotalMass;
}

