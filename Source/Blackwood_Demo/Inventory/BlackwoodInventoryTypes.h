#pragma once

#include "CoreMinimal.h"
#include "BlackwoodInventoryTypes.generated.h"

class UBlackwoodItemDefinition;

USTRUCT(BlueprintType)
struct BLACKWOOD_DEMO_API FBlackwoodInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UBlackwoodItemDefinition> ItemDefinition = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 Quantity = 0;
};
