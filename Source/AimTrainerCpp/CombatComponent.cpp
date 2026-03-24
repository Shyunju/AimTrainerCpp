// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "MyCharacter.h"
#include "EnemyTarget.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
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
	CurrentAmmo = MaxAmmo;
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
bool UCombatComponent::CanFire() const
{
	return !bIsReloading && CurrentAmmo > 0;
}
void UCombatComponent::FireTarget()
{
	/*AActor* OwnerCharacter = GetOwner();
	if (!OwnerCharacter) return;*/

	if (!CanFire())
	{
		if (CurrentAmmo <= 0 && !bIsReloading)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Empty ammo - reload(R)"));
		}
		return;
	}

	AMyCharacter* OwnerCharacter = Cast<AMyCharacter>(GetOwner());
	if (!OwnerCharacter || !CurrentWeaponMesh) return;
	CurrentAmmo--;

	OwnerCharacter->PlayFireAnimation();

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
	if (!OwnerCharacter->FPSCamera) return;

	FVector Start = OwnerCharacter->FPSCamera->GetComponentLocation();
	FVector Direction = OwnerCharacter->FPSCamera->GetForwardVector();
	FVector End = Start + (Direction * TraceRange);

	/*FVector CameraLocation;
	FRotator CameraRotation;
	OwnerCharacter->GetActorEyesViewPoint(CameraLocation, CameraRotation);

	FVector Start = CameraLocation;
	FVector End = Start + (CameraRotation.Vector() * TraceRange);*/


	//충돌검사
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);
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
		/*UE_LOG(LogTemp, Warning, TEXT("Hit Target : %s"), *HitResult.GetActor()->GetName());
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 12, FColor::Red, false, 2.0f);*/

		FVector ImpactPoint = HitResult.ImpactPoint;
		FVector ImpactNormal = HitResult.ImpactNormal;

		if (ImpactConcreteEffect)
		{
			UNiagaraComponent* ImpactComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ImpactConcreteEffect,
				ImpactPoint,
				ImpactNormal.Rotation()
			);

			if (ImpactComp)
			{
				TArray<FVector> ImpactPositions;
				ImpactPositions.Add(ImpactPoint);
				TArray<FVector> ImpactNormals;
				ImpactNormals.Add(ImpactNormal);
				UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPosition(
					ImpactComp,
					FName("User.ImpactPositions"),
					ImpactPositions
				);
				UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPosition(
					ImpactComp,
					FName("User.ImpactNormals"),
					ImpactNormals
				);
			}

		}
		if (ImpactDecalEffect)
		{

			UNiagaraComponent* DecalComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ImpactDecalEffect,
				ImpactPoint,
				ImpactNormal.Rotation()
			);

			if (DecalComp)
			{
				TArray<FVector> ImpactPositions;
				ImpactPositions.Add(ImpactPoint);
				TArray<FVector> ImpactNormals;
				ImpactNormals.Add(ImpactNormal);

				TArray<int32> ImpactSurfaces;
				ImpactSurfaces.Add(2); //1character , 2 concrete

				UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayPosition(
					DecalComp,
					FName("User.ImpactPositions"),
					ImpactPositions
				);
				UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(
					DecalComp,
					FName("User.ImpactNormals"),
					ImpactNormals
				);
				UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayInt32(
					DecalComp,
					FName("User.ImpactSurfaces"),
					ImpactSurfaces
				);

				DecalComp->SetVariableQuat(FName("User.Orientation"), (-ImpactNormal).ToOrientationQuat());
				DecalComp->SetVectorParameter(FName("User.MusslePosition"), MuzzleLocation);
				DecalComp->SetIntParameter(FName("User.NumberofHits"), 1);
				DecalComp->SetIntParameter(FName("User.SurfaceType"), 1);
				DecalComp->SetVectorParameter(FName("User.StartOffset"), FVector(0.0f, 0.0f, 2.0f));
				DecalComp->SetBoolParameter(FName("User.Trigger"), true);
			}
		}
	}

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.0f, 0, 1.0f);

	if (AEnemyTarget* HitEnemy = Cast<AEnemyTarget>(HitResult.GetActor()))
	{
		HitEnemy->OnHit(HitResult.PhysMaterial.Get());
	}
}

void UCombatComponent::Reload()
{
	if (bIsReloading || CurrentAmmo >= MaxAmmo)	return;

	AMyCharacter* OwnerCharacter = Cast<AMyCharacter>(GetOwner());
	if (!OwnerCharacter) return;

	float ActualReloadTime = ReloadTime;

	if (OwnerCharacter->ArmsFireMontage)
	{
		int32 SectionIndex = OwnerCharacter->ArmsFireMontage->GetSectionIndex(OwnerCharacter->ReloadSectionName);
		if (SectionIndex != INDEX_NONE)
		{
			ActualReloadTime = OwnerCharacter->ArmsFireMontage->GetSectionLength(SectionIndex);
		}
	}

	bIsReloading = true;
	OwnerCharacter->PlayReloadAnimation();
	GetWorld()->GetTimerManager().SetTimer(ReloadTiemerHandle, this, &UCombatComponent::FinishReloading, ActualReloadTime, false);

}

void UCombatComponent::FinishReloading()
{
	bIsReloading = false;
	CurrentAmmo = MaxAmmo;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Reload complete"));
}

