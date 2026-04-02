// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AimTrainerTypes.h"
#include "AimTranerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AIMTRAINERCPP_API AAimTranerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAimTranerGameMode();

protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void StartGame();

	void EndGame();

	void AddHitScore(EHitZone HitZone);

	void OnTargetKilled();
	void OnTargetExpired();
	void AddShotCount();

	/*UFUNCTION(BlueprintImplementableEvent, Category = "GameFlow")
	void OnGameOver();*/
	UFUNCTION(BlueprintNativeEvent, Category = "GameFlow")
	void OnGameOver();
	virtual void OnGameOver_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|UI")
	TSubclassOf<UUserWidget> ResultWidgetClass;

	UPROPERTY()
	UUserWidget* ResultWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Config")
	float GameTimeLimit = 60.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Stats")
	float TimeRemaining;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Config")
	float ShotPenaltyValue = 2.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Stats")
	bool bIsGameStrated = false;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Stats")
	int32 TotalScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Stats")
	int32 CurrentCombo = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Stats")
	int32 TotalShotsFired = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Stats")
	int32 HeadHits = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Stats")
	int32 BodyHits = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Stats")
	int32 LimbHits = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Stats")
	float FinalSocre = 0.0f;
private:
	void UpdateTimer();

	FTimerHandle GameTimerHandle;
};
