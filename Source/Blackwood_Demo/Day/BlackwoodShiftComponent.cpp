#include "Day/BlackwoodShiftComponent.h"

#include "Tasks/BlackwoodWorkOrderComponent.h"

UBlackwoodShiftComponent::UBlackwoodShiftComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(false);
}

EBlackwoodShiftState UBlackwoodShiftComponent::GetShiftState() const
{
	return ShiftState;
}

bool UBlackwoodShiftComponent::IsOnDuty() const
{
	return ShiftState == EBlackwoodShiftState::OnDuty;
}

bool UBlackwoodShiftComponent::HasFiledReport() const
{
	return ShiftState == EBlackwoodShiftState::ReportFiled;
}

bool UBlackwoodShiftComponent::FileDailyReport(
	UBlackwoodWorkOrderComponent* WorkOrderComponent)
{
	if (!IsValid(WorkOrderComponent) || HasFiledReport())
	{
		return false;
	}

	const FBlackwoodWorkOrder WorkOrder =
		WorkOrderComponent->GetCurrentWorkOrder();
	if (WorkOrder.WorkOrderId.IsNone()
		|| WorkOrder.State != EBlackwoodWorkOrderState::Completed)
	{
		return false;
	}

	ShiftState = EBlackwoodShiftState::ReportFiled;
	return true;
}

FText UBlackwoodShiftComponent::GetShiftSummary() const
{
	if (HasFiledReport())
	{
		return NSLOCTEXT(
			"BlackwoodShift",
			"ReportFiledSummary",
			"Shift Status: Report Filed\nReturn to your quarters.");
	}

	return NSLOCTEXT(
		"BlackwoodShift",
		"OnDutySummary",
		"Shift Status: On Duty");
}
