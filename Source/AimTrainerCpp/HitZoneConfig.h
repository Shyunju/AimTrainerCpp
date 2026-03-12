// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AimTrainerTypes.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "HitZoneConfig.generated.h"

/**
 * 
 */
UCLASS()
class AIMTRAINERCPP_API UHitZoneConfig : public UDataAsset
{
	GENERATED_BODY()

public :
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TMap<UPhysicalMaterial*, EHitZone> MaterialMapping;

	EHitZone GetHitZone(UPhysicalMaterial* PhysMat) const
	{
		if (PhysMat && MaterialMapping.Contains(PhysMat))
		{
			return MaterialMapping[PhysMat];
		}
		return EHitZone::None;
	}
	
};
