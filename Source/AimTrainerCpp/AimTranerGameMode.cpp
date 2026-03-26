// Fill out your copyright notice in the Description page of Project Settings.

#include "TimerManager.h"
#include "StartButton.h"
#include "AimTranerGameMode.h"

AAimTranerGameMode::AAimTranerGameMode()
{

}

void AAimTranerGameMode::BeginPlay()
{
	Super::BeginPlay();

	TimeRemaining = GameTimeLimit;
}

void AAimTranerGameMode::StartGame()
{
	if (bIsGameStrated)
	{
		return;
	}

	bIsGameStrated = true;
	TimeRemaining = GameTimeLimit;

	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AAimTranerGameMode::UpdateTimer, 1.0f, true);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("game started"));
}
void AAimTranerGameMode::EndGame()
{
	bIsGameStrated = false;
	GetWorldTimerManager().ClearTimer(GameTimerHandle);

}
void AAimTranerGameMode::UpdateTimer()
{
	if (!bIsGameStrated)	return;

	TimeRemaining -= 1.0f;

	if (TimeRemaining <= 0.0f)
	{
		TimeRemaining = 0.0f;
		EndGame();
	}
}

