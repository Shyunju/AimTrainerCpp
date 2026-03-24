// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnemyTarget.h"
#include "TargetSpawner.h"
#include "TimerManager.h"


// Sets default values
ATargetSpawner::ATargetSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(RootComponent);
	SpawnArea->SetBoxExtent(FVector(1000.0f, 1000.0f, 200.0f));
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ATargetSpawner::BeginPlay()
{
	Super::BeginPlay();
	ScheduleNextSpawn();
	
}

// Called every frame
void ATargetSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ATargetSpawner::GetRandomPointInBox() const
{
	if (!SpawnArea)
	{
		return GetActorLocation();
	}

	FVector Origin = SpawnArea->GetComponentLocation();
	FVector Extent = SpawnArea->GetScaledBoxExtent();

	float RandomX = FMath::RandRange(Origin.X - Extent.X, Origin.X + Extent.X);
	float RandomY = FMath::RandRange(Origin.Y - Extent.Y, Origin.Y + Extent.Y);

	FVector Start(RandomX, RandomY, Origin.Z + Extent.Z); //스폰박스 천장
	FVector End(RandomX, RandomY, Origin.Z - Extent.Z); //스폰박스 바닥

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(true);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);  //레이가 최초로 닿는 곳 (땅)

	float ZPos = Origin.Z - Extent.Z;

	if (bHit)
	{
		ZPos = HitResult.ImpactPoint.Z;
	}

	float TargetHalfHeight = 90.0f;
	if (TargetClass)
	{
		if (AEnemyTarget* DefaultTarget = Cast<AEnemyTarget>(TargetClass->GetDefaultObject()))
		{
			if (DefaultTarget->CapsuleComponent)
			{
				TargetHalfHeight = DefaultTarget->CapsuleComponent->GetScaledCapsuleHalfHeight();
			}
		}
	}

	return FVector(RandomX, RandomY, ZPos + TargetHalfHeight);
}
void ATargetSpawner::SpawnTarget()
{
	if (ActiveTargets.Num() >= MaxActiveTargets)
	{
		ScheduleNextSpawn();
		return;
	}
	if (TargetClass)
	{
		FVector SpawnLocation = GetRandomPointInBox();
		FRotator SpawnRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AEnemyTarget* NewTarget = GetWorld()->SpawnActor<AEnemyTarget>(TargetClass, SpawnLocation, SpawnRotation, SpawnParams);

		if (NewTarget)
		{
			ActiveTargets.Add(NewTarget);
			NewTarget->OnDestroyed.AddDynamic(this, &ATargetSpawner::OnTargetDestroyed);

		}
		
	}
	ScheduleNextSpawn();

}

void ATargetSpawner::ScheduleNextSpawn()
{
	float SpawnDelay = FMath::RandRange(MinSpawnDelay, MaxSpawnDelay);
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ATargetSpawner::SpawnTarget, SpawnDelay, false);
}
void ATargetSpawner::OnTargetDestroyed(AActor* DestroyedActor)
{
	AEnemyTarget* Target = Cast<AEnemyTarget>(DestroyedActor);
	if (Target)
	{
		ActiveTargets.Remove(Target);
	}
}


