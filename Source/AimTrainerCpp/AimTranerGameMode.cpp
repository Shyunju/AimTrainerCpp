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

	if (APlayerController* Pc = GetWorld()->GetFirstPlayerController())
	{
		Pc->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		Pc->SetInputMode(InputMode);
	}
}

void AAimTranerGameMode::StartGame()
{
	if (bIsGameStrated)
	{
		return;
	}

	bIsGameStrated = true;
	TimeRemaining = GameTimeLimit;

	if (APlayerController* Pc = GetWorld()->GetFirstPlayerController())
	{
		Pc->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		Pc->SetInputMode(InputMode);
	}

	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &AAimTranerGameMode::UpdateTimer, 1.0f, true);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("game started"));
}
void AAimTranerGameMode::AddHitScore(EHitZone HitZone)
{
	if (!bIsGameStrated)	return;

	int32 HitScore = 0;

	switch (HitZone)
	{
	case EHitZone::Head:
		HitScore = 90;
		HeadHits++;
		break;
	case EHitZone::Body:
		HitScore = 40;
		BodyHits++;
		break;
	case EHitZone::Limbs:
		HitScore = 20;
		LimbHits++;
		break;
	default:
		break;
	}

	TotalScore += HitScore;
}

void AAimTranerGameMode::OnTargetKilled()
{
	if (!bIsGameStrated)	return;

	TotalScore += (10 + CurrentCombo);
	CurrentCombo++;
}
void AAimTranerGameMode::OnTargetExpired()
{
	if (!bIsGameStrated)	return;
	CurrentCombo = 0;
}
void AAimTranerGameMode::AddShotCount()
{
	if (bIsGameStrated)
	{
		TotalShotsFired++;
	}
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


void AAimTranerGameMode::EndGame()
{
	bIsGameStrated = false;
	GetWorldTimerManager().ClearTimer(GameTimerHandle);

	float ShotPenalty = TotalShotsFired * ShotPenaltyValue;
	FinalSocre = (float)TotalScore - ShotPenalty;

	if (APlayerController* Pc = GetWorld()->GetFirstPlayerController())
	{
		Pc->bShowMouseCursor = true;
		FInputModeUIOnly InputMode;
		Pc->SetInputMode(InputMode);
	}

	OnGameOver();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Game over"));
	}
}