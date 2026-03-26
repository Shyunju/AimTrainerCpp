// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StartButton.generated.h"

class UBoxComponent;
class UTextRenderComponent;

UCLASS()
class AIMTRAINERCPP_API AStartButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStartButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	void OnHit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTextRenderComponent* TextLabel;

};
