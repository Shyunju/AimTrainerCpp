#pragma once

#include "CoreMinimal.h"
#include "AimTrainerTypes.generated.h"

UENUM(BlueprintType)
enum class EHitZone : uint8
{
	None UMETA(DisplayName = "None"),
	Head UMETA(DisplayName = "Head"),
	Body UMETA(DisplayName = "Body"),
	Limbs UMETA(DisplayName = "Limbs")
};