// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TargetSpawner.generated.h"

class UBoxComponent;
class AEnemyTarget;


UCLASS()
class AIMTRAINERCPP_API ATargetSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetRandomPointInBox() const;

	void SpawnTarget();
	void ScheduleNextSpawn();

	UFUNCTION()
	void OnTargetDestroyed(AActor* DestroyedActor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Spawning")
	UBoxComponent* SpawnArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AEnemyTarget> TargetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning|Config")
	float MinSpawnDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning|Config")
	float MaxSpawnDelay = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning|Config")
	int32 MaxActiveTargets = 5;


private:
	UPROPERTY()
	TArray<AEnemyTarget*> ActiveTargets;
	
	FTimerHandle SpawnTimerHandle;

};
