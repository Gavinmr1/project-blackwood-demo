#pragma once

#include "CoreMinimal.h"
#include "BlackwoodShiftTypes.generated.h"

UENUM(BlueprintType)
enum class EBlackwoodShiftState : uint8
{
	OnDuty,
	ReportFiled
};
