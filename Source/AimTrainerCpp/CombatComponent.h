// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class UNiagaraSystem;
class USkeletalMeshComponenet;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AIMTRAINERCPP_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FireTarget();

	void Reload();

	bool CanFire() const;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float TraceRange = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Combat|Debug")
	bool bShowMuzzleDebugLine = true;

	UPROPERTY()
	USkeletalMeshComponent* CurrentWeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Combat|VFX")
	UNiagaraSystem* MuzzleFalshEffect;

	UPROPERTY(EditAnywhere, Category = "Combat|VFX")
	float MuzzleScale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Combat|VFX")
	UNiagaraSystem* TracerEffect;

	UPROPERTY(EditAnywhere, Category = "Combat|VFX")
	UNiagaraSystem* ImpactConcreteEffect;

	UPROPERTY(EditAnywhere, Category = "Combat|VFX")
	UNiagaraSystem* ImpactDecalEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Ammo")
	int32 MaxAmmo = 30;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Ammo")
	int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Ammo")
	float ReloadTime = 1.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Stats")
	int32 TotalShotsFired = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat|Ammo")
	bool bIsReloading = false;

private :
	void FinishReloading();

	FTimerHandle ReloadTiemerHandle;
		
};
