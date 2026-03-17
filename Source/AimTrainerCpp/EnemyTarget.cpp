// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "HitZoneConfig.h"
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

	FString ZoneString = "";
	FColor MessageColor = FColor::White;
	FString MatName = PhysMat ? PhysMat->GetName() : TEXT("None");

	switch (HitZone)
	{
	case EHitZone::Head:
		ZoneString = "head";
		MessageColor = FColor::Red;
		break;
	case EHitZone::Body:
		ZoneString = "body";
		MessageColor = FColor::Green;
		break;
	case EHitZone::Limbs:
		ZoneString = "limbs";
		MessageColor = FColor::Yellow;
		break;
	default:
		ZoneString = "unkwon/none";
		MessageColor = FColor::White;
		break;
	}
	if (GEngine)
	{
		FString DebugMessage = FString::Printf(TEXT("Hit: %s (PhysMat : %s)"), *ZoneString, *MatName);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, MessageColor, DebugMessage);
	}
}

