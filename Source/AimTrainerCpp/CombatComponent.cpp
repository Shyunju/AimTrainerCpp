// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bShowMuzzleDebugLine && CurrentWeaponMesh)
	{
		FVector MuzzleLocation = CurrentWeaponMesh->GetSocketLocation(FName("Muzzle"));
		FVector MuzzleForward = CurrentWeaponMesh->GetSocketQuaternion(FName("Muzzle")).GetForwardVector();

		FVector End = MuzzleLocation + (MuzzleForward * TraceRange);

		DrawDebugLine(GetWorld() , MuzzleLocation, End, FColor::Yellow, false, -1.0f, 0, 1.0f);

		DrawDebugSphere(GetWorld(), MuzzleLocation, 2.0f, 8, FColor::Red, false, -1.0f);
		//FireTarget();


	}
}
void UCombatComponent::FireTarget()
{

}

