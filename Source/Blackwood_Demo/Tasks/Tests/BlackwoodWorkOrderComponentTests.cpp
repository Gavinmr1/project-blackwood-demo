#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Tasks/BlackwoodWorkOrderComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FBlackwoodWorkOrderComponentTest,
	"Blackwood.Tasks.WorkOrder.Foundation",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FBlackwoodWorkOrderComponentTest::RunTest(const FString& Parameters)
{
	UBlackwoodWorkOrderComponent* WorkOrders = NewObject<UBlackwoodWorkOrderComponent>();
	const FName RepairId(TEXT("RepairTrailSign"));
	const FText DisplayName = FText::FromString(TEXT("Repair Trail Sign"));
	const FText Description =
		FText::FromString(TEXT("Inspect and repair the damaged trail sign."));

	TestEqual(
		TEXT("Empty state has the expected summary"),
		WorkOrders->GetWorkOrderSummary().ToString(),
		FString(TEXT("No Active Work Order")));
	TestFalse(
		TEXT("Empty work-order ID is rejected"),
		WorkOrders->AssignWorkOrder(NAME_None, DisplayName, Description));
	TestFalse(
		TEXT("Empty display name is rejected"),
		WorkOrders->AssignWorkOrder(RepairId, FText::GetEmpty(), Description));
	TestTrue(
		TEXT("Valid work order can be assigned"),
		WorkOrders->AssignWorkOrder(RepairId, DisplayName, Description));
	TestEqual(
		TEXT("Assigned work order is active"),
		WorkOrders->GetCurrentWorkOrder().State,
		EBlackwoodWorkOrderState::Active);
	TestTrue(TEXT("HasActiveWorkOrder succeeds"), WorkOrders->HasActiveWorkOrder());
	TestTrue(
		TEXT("Correct current ID succeeds"),
		WorkOrders->IsCurrentWorkOrder(RepairId));
	TestFalse(
		TEXT("Wrong current ID fails"),
		WorkOrders->IsCurrentWorkOrder(TEXT("UnrelatedWorkOrder")));
	TestFalse(
		TEXT("Duplicate active assignment is rejected"),
		WorkOrders->AssignWorkOrder(RepairId, DisplayName, Description));
	TestFalse(
		TEXT("Another work order cannot replace an active work order"),
		WorkOrders->AssignWorkOrder(
			TEXT("OtherWorkOrder"),
			FText::FromString(TEXT("Other Work Order")),
			FText::GetEmpty()));

	const FString ActiveSummary = WorkOrders->GetWorkOrderSummary().ToString();
	TestTrue(
		TEXT("Active summary contains display name"),
		ActiveSummary.Contains(TEXT("Repair Trail Sign")));
	TestTrue(
		TEXT("Active summary contains Active state"),
		ActiveSummary.Contains(TEXT("Status: Active")));

	TestTrue(
		TEXT("Active work order can be completed"),
		WorkOrders->CompleteCurrentWorkOrder());
	TestEqual(
		TEXT("Completed state is stored"),
		WorkOrders->GetCurrentWorkOrder().State,
		EBlackwoodWorkOrderState::Completed);
	TestFalse(
		TEXT("Completed work order is not active"),
		WorkOrders->HasActiveWorkOrder());
	TestFalse(
		TEXT("Completed work order cannot be completed twice"),
		WorkOrders->CompleteCurrentWorkOrder());
	TestFalse(
		TEXT("Completed work order cannot be reset by duplicate assignment"),
		WorkOrders->AssignWorkOrder(RepairId, DisplayName, Description));
	TestTrue(
		TEXT("Completed summary contains Completed state"),
		WorkOrders->GetWorkOrderSummary().ToString().Contains(TEXT("Status: Completed")));

	return true;
}

#endif
