#include "Tasks/BlackwoodWorkOrderComponent.h"

UBlackwoodWorkOrderComponent::UBlackwoodWorkOrderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(false);
}

bool UBlackwoodWorkOrderComponent::AssignWorkOrder(
	const FName WorkOrderId,
	const FText DisplayName,
	const FText Description)
{
	if (WorkOrderId.IsNone() || DisplayName.IsEmpty())
	{
		return false;
	}

	if (HasActiveWorkOrder() || IsCurrentWorkOrder(WorkOrderId))
	{
		return false;
	}

	CurrentWorkOrder.WorkOrderId = WorkOrderId;
	CurrentWorkOrder.DisplayName = DisplayName;
	CurrentWorkOrder.Description = Description;
	CurrentWorkOrder.State = EBlackwoodWorkOrderState::Active;
	return true;
}

bool UBlackwoodWorkOrderComponent::CompleteCurrentWorkOrder()
{
	if (!HasActiveWorkOrder())
	{
		return false;
	}

	CurrentWorkOrder.State = EBlackwoodWorkOrderState::Completed;
	return true;
}

bool UBlackwoodWorkOrderComponent::HasActiveWorkOrder() const
{
	return !CurrentWorkOrder.WorkOrderId.IsNone()
		&& CurrentWorkOrder.State == EBlackwoodWorkOrderState::Active;
}

bool UBlackwoodWorkOrderComponent::IsCurrentWorkOrder(const FName WorkOrderId) const
{
	return !WorkOrderId.IsNone()
		&& !CurrentWorkOrder.WorkOrderId.IsNone()
		&& CurrentWorkOrder.WorkOrderId == WorkOrderId;
}

FBlackwoodWorkOrder UBlackwoodWorkOrderComponent::GetCurrentWorkOrder() const
{
	return CurrentWorkOrder;
}

FText UBlackwoodWorkOrderComponent::GetWorkOrderSummary() const
{
	if (CurrentWorkOrder.WorkOrderId.IsNone())
	{
		return FText::FromString(TEXT("No Active Work Order"));
	}

	const TCHAR* StateText =
		CurrentWorkOrder.State == EBlackwoodWorkOrderState::Completed
			? TEXT("Completed")
			: TEXT("Active");

	return FText::Format(
		NSLOCTEXT(
			"BlackwoodWorkOrders",
			"WorkOrderSummary",
			"Current Work Order\n{0}\n{1}\n\nStatus: {2}"),
		CurrentWorkOrder.DisplayName,
		CurrentWorkOrder.Description,
		FText::FromString(StateText));
}
