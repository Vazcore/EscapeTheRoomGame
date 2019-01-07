// Alex Gaba, 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Classes/PhysicsEngine/PhysicsHandleComponent.h"
#include "Engine/Classes/Components/InputComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEST1_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void UpdateViewPoint();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AActor* Owner;
	AActor* PointedActor;
	FVector PlayerViewPoint = FVector();
	FRotator PlayerRotatorViewPoint = FRotator();
	FString LocationX;
	FString LocationY;
	FString LocationZ;
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void GrabReleased();
	void FindPhysicsHandleComponent();
	void SetupInputComponent();
	FHitResult GetFirstPhysicsBodyInReach();
	FVector GetUpdatedLineTraceEnd();
};
