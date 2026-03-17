// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AimTrainerTypes.h"
#include "EnemyTarget.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UHitZoneConfig;
class UPhysicalMaterial;

UCLASS()
class AIMTRAINERCPP_API AEnemyTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void ProcessHit(UPhysicalMaterial* PhysMat);
	virtual void Die(bool bWasKilled);
	void StartWarning();
	void OnLifeSpanExpired();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UHitZoneConfig* HitZoneConfig;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stats")
	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stats")
	float MaxLifeTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Stats")
	float WarningTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Visual")
	FLinearColor WarningColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Visual")
	float RagdollLifespan = 3.0f;

	void OnHit(UPhysicalMaterial* PhysMat);

private:
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;
	FTimerHandle LifeSpanTimerHandle;
	FTimerHandle WarningTimerHandle;
	FTimerHandle DestroyTimerHandle;

};
