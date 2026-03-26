// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game|Config")
	float GameTimeLimit = 60.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Stats")
	float TimeRemaining;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game|Stats")
	bool bIsGameStrated = false;

private:
	void UpdateTimer();

	FTimerHandle GameTimerHandle;
};
