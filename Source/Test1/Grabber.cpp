// Alex Gaba, 2019

#include "Grabber.h"
#include "Engine/Classes/GameFramework/Actor.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Classes/Components/PrimitiveComponent.h"
#include "Engine/Public/DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::UpdateViewPoint()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPoint, OUT PlayerRotatorViewPoint);
	LocationX = FString::SanitizeFloat(PlayerViewPoint.X);
	LocationY = FString::SanitizeFloat(PlayerViewPoint.Y);
	LocationZ = FString::SanitizeFloat(PlayerViewPoint.Z);
	return;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetUpdatedLineTraceEnd());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key was pressed"));
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	if (Hit.GetActor() != nullptr) {
		UPrimitiveComponent* GrabComponent = Hit.GetComponent();
		PhysicsHandle->GrabComponent(
			GrabComponent,
			NAME_None,
			GrabComponent->GetOwner()->GetActorLocation(),
			true // allow to rotate
		);
	}
	
}

void UGrabber::GrabReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key was released"));
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = Owner->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s has PhysicsHandle component"), *Owner->GetActorLabel());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Actor: %s doesn't have PhysicsHandle component"), *Owner->GetActorLabel());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = Owner->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Actor: %s has InputComponent component"), *Owner->GetActorLabel());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabReleased);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Actor: %s doesn't have InputComponent component"), *Owner->GetActorLabel());
	}
}



FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	UpdateViewPoint();
	FVector LineTraceEnd = GetUpdatedLineTraceEnd();
	/*
	DrawDebugLine(
		GetWorld(),
		PlayerViewPoint,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);
	*/

	//UE_LOG(LogTemp, Warning, TEXT("Location X: %s, Location Y: %s, Location Z: %s"), *LocationX, *LocationY, *LocationZ);

	FHitResult Hit;
	FCollisionQueryParams AdditionalCollisionParams = FCollisionQueryParams(FName(TEXT("")), false, Owner);
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPoint,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		AdditionalCollisionParams
	);

	PointedActor = Hit.GetActor();

	if (PointedActor) {
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *PointedActor->GetActorLabel());
	}
	return Hit;
}

FVector UGrabber::GetUpdatedLineTraceEnd()
{
	UpdateViewPoint();
	FVector LineTraceDirection = PlayerRotatorViewPoint.Vector() * Reach;
	FVector LineTraceEnd = (PlayerViewPoint + LineTraceDirection);
	return LineTraceEnd;
}

