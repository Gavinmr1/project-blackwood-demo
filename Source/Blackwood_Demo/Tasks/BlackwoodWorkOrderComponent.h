#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tasks/BlackwoodWorkOrderTypes.h"
#include "BlackwoodWorkOrderComponent.generated.h"

UCLASS(ClassGroup = (Blackwood), meta = (BlueprintSpawnableComponent))
class BLACKWOOD_DEMO_API UBlackwoodWorkOrderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBlackwoodWorkOrderComponent();

	UFUNCTION(BlueprintCallable, Category = "Blackwood|Tasks")
	bool AssignWorkOrder(FName WorkOrderId, FText DisplayName, FText Description);

	UFUNCTION(BlueprintCallable, Category = "Blackwood|Tasks")
	bool CompleteCurrentWorkOrder();

	UFUNCTION(BlueprintPure, Category = "Blackwood|Tasks")
	bool HasActiveWorkOrder() const;

	UFUNCTION(BlueprintPure, Category = "Blackwood|Tasks")
	bool IsCurrentWorkOrder(FName WorkOrderId) const;

	UFUNCTION(BlueprintPure, Category = "Blackwood|Tasks")
	FBlackwoodWorkOrder GetCurrentWorkOrder() const;

	UFUNCTION(BlueprintPure, Category = "Blackwood|Tasks")
	FText GetWorkOrderSummary() const;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Work Order", meta = (AllowPrivateAccess = "true"))
	FBlackwoodWorkOrder CurrentWorkOrder;
};
