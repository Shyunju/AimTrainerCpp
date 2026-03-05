// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"

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

		//DrawDebugLine(GetWorld() , MuzzleLocation, End, FColor::Yellow, false, -1.0f, 0, 1.0f);

		//DrawDebugSphere(GetWorld(), MuzzleLocation, 2.0f, 8, FColor::Red, false, -1.0f);
		


	}
}
void UCombatComponent::FireTarget()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	FVector MuzzleLocation = CurrentWeaponMesh->GetSocketLocation(FName("Muzzle"));
	FRotator MuzzleRotation = CurrentWeaponMesh->GetSocketRotation(FName("Muzzle"));

	if (MuzzleFalshEffect)
	{
		UNiagaraComponent* MuzzleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			MuzzleFalshEffect,
			CurrentWeaponMesh,
			FName("Muzzle"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);

		if (MuzzleComp)
		{
			MuzzleComp->SetRelativeScale3D(FVector(MuzzleScale, MuzzleScale, MuzzleScale));
			MuzzleComp->SetBoolParameter(FName("User.Trigger"), true);
		}
	}

	FVector CameraLocation;
	FRotator CameraRotation;
	Owner->GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector Start = CameraLocation;
	FVector End = Start + (CameraRotation.Vector() * TraceRange);


	//충돌검사
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);
	Params.bReturnPhysicalMaterial = true;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);
	FVector TargetPoint = bHit ? HitResult.ImpactPoint : End;
	if (TracerEffect)
	{
		UNiagaraComponent* TracerComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			TracerEffect,
			MuzzleLocation,
			MuzzleRotation
		);
		if (TracerComp)
		{
			TArray<FVector> ImpactPositions;
			ImpactPositions.Add(TargetPoint);
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(
				TracerComp,
				FName("User.ImpactPositions"),
				ImpactPositions
			);
			TracerComp->SetBoolParameter(FName("User.Trigger"), true);
		}
	}

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Target : %s"), *HitResult.GetActor()->GetName());
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 12, FColor::Red, false, 2.0f);
	}

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 1.0f);
}

