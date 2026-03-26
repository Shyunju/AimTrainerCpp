// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "AimTranerGameMode.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "StartButton.h"

// Sets default values
AStartButton::AStartButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetBoxExtent(FVector(150.0f, 20.0f, 50.0f));
	CollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	TextLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextLabel"));
	TextLabel->SetupAttachment(RootComponent);
	TextLabel->SetText(FText::FromString("SHOOT TO START"));
	TextLabel->SetHorizontalAlignment(EHTA_Center);
	TextLabel->SetVerticalAlignment(EVRTA_TextCenter);
	TextLabel->SetWorldSize(100.0f);

	TextLabel->SetRelativeLocation(FVector::ZeroVector);
}

// Called when the game starts or when spawned
void AStartButton::BeginPlay()
{
	Super::BeginPlay();
	
}
void AStartButton::OnHit()
{
	AAimTranerGameMode* GameMode = Cast<AAimTranerGameMode>(UGameplayStatics::GetGameMode(GetWorld())); //like singletone
	if (GameMode)
	{
		GameMode->StartGame();

		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

