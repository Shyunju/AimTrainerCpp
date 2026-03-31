// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "HitZoneConfig.h"
#include "Kismet/GameplayStatics.h"
#include "AimTranerGameMode.h"
#include "EnemyTarget.h"

// Sets default values
AEnemyTarget::AEnemyTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->InitCapsuleSize(40.0f, 90.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = CapsuleComponent;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName(TEXT("CharacterMesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);

}

// Called when the game starts or when spawned
void AEnemyTarget::BeginPlay()
{
	Super::BeginPlay();
	
	if (Mesh && Mesh->GetNumMaterials() > 0)
	{
		DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamic(0);
	}

	float WarningStartTime = FMath::Max(0.1f, MaxLifeTime - WarningTime);
	GetWorldTimerManager().SetTimer(WarningTimerHandle, this, &AEnemyTarget::StartWarning, WarningStartTime, false);

	GetWorldTimerManager().SetTimer(LifeSpanTimerHandle, this, &AEnemyTarget::OnLifeSpanExpired, MaxLifeTime, false);
}

// Called every frame
void AEnemyTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AEnemyTarget::OnHit(UPhysicalMaterial* PhysMat)
{
	ProcessHit(PhysMat);
}
void AEnemyTarget::ProcessHit(UPhysicalMaterial* PhysMat)
{
	if (!HitZoneConfig)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Warning: hitzoneconfig is not assinged to enemytarget"));
		}
		return;
	}
	EHitZone HitZone = HitZoneConfig->GetHitZone(PhysMat);

	AAimTranerGameMode* GameMode = Cast<AAimTranerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->AddHitScore(HitZone);
	}
	float DamageAmount = 0.0f;
	FString ZoneString = "";
	FColor MessageColor = FColor::White;
	//FString MatName = PhysMat ? PhysMat->GetName() : TEXT("None");

	switch (HitZone)
	{
	case EHitZone::Head:
		DamageAmount = 100.0f;
		ZoneString = "head";
		MessageColor = FColor::Red;
		break;
	case EHitZone::Body:
		DamageAmount = 50.0f;
		ZoneString = "body";
		MessageColor = FColor::Green;
		break;
	case EHitZone::Limbs:
		DamageAmount = 35.0f;
		ZoneString = "limbs";
		MessageColor = FColor::Yellow;
		break;
	default:
		ZoneString = "unkwon/none";
		MessageColor = FColor::White;
		break;
	}
	Health -= DamageAmount;
	if (GEngine)
	{
		FString DebugMessage = FString::Printf(TEXT("Hit: %s | damage : %.f | health: %.f"), *ZoneString, DamageAmount, FMath::Max(0.0f, Health));
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, MessageColor, DebugMessage);
	}
	if (Health <= 0)
	{
		Die(true);
	}

}

void AEnemyTarget::StartWarning()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("Paint Tint"), WarningColor);

	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("target warning: hurry up"));
	}
}

void AEnemyTarget::Die(bool bWasKilled)
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	AAimTranerGameMode* GameMode = Cast<AAimTranerGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		if (bWasKilled) GameMode->OnTargetKilled();
		else GameMode->OnTargetExpired();
	}
	if (bWasKilled)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("target killed - ragdoll"));
		}
		if (Mesh)
		{
			Mesh->SetSimulatePhysics(true);
			Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			Mesh->SetCollisionProfileName(TEXT("Ragdoll"));

			if (CapsuleComponent)
			{
				CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}

			FTimerDelegate DestroyDelegate;
			DestroyDelegate.BindLambda([this]() {
				Destroy();
			});
			GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyDelegate, RagdollLifespan, false);
		}
		else
		{
			Destroy();
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Silver, TEXT("target edpired - fail"));
		}
		Destroy();

	}

}

void AEnemyTarget::OnLifeSpanExpired()
{
	Die(false);
}

