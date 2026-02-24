// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeRotaterComponent.h"

// Sets default values for this component's properties
UCubeRotaterComponent::UCubeRotaterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	RotationSpeed = FRotator(0.0f, 90.f, 0.0f);
}


// Called when the game starts
void UCubeRotaterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCubeRotaterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AActor* OwnerActor = GetOwner();

	if (OwnerActor)
	{
		FRotator NewRotation = RotationSpeed * DeltaTime;
		OwnerActor->AddActorLocalRotation(NewRotation);
	}
}

