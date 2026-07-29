#pragma once

#include "CoreMinimal.h"
#include "BlackwoodWorkOrderTypes.generated.h"

UENUM(BlueprintType)
enum class EBlackwoodWorkOrderState : uint8
{
	Inactive,
	Active,
	Completed
};

USTRUCT(BlueprintType)
struct BLACKWOOD_DEMO_API FBlackwoodWorkOrder
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Work Order")
	FName WorkOrderId;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Work Order")
	FText DisplayName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Work Order")
	FText Description;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Work Order")
	EBlackwoodWorkOrderState State = EBlackwoodWorkOrderState::Inactive;
};
