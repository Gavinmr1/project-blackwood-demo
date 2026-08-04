#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Day/BlackwoodShiftComponent.h"
#include "Tasks/BlackwoodWorkOrderComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FBlackwoodShiftComponentTest,
	"Blackwood.Day.Shift.ReportFoundation",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FBlackwoodShiftComponentTest::RunTest(const FString& Parameters)
{
	UBlackwoodShiftComponent* Shift = NewObject<UBlackwoodShiftComponent>();
	UBlackwoodWorkOrderComponent* WorkOrders =
		NewObject<UBlackwoodWorkOrderComponent>();

	TestEqual(
		TEXT("Shift begins OnDuty"),
		Shift->GetShiftState(),
		EBlackwoodShiftState::OnDuty);
	TestTrue(TEXT("IsOnDuty initially succeeds"), Shift->IsOnDuty());
	TestFalse(TEXT("Report is initially unfiled"), Shift->HasFiledReport());
	TestTrue(
		TEXT("Initial summary contains On Duty"),
		Shift->GetShiftSummary().ToString().Contains(TEXT("On Duty")));
	TestFalse(
		TEXT("Null work-order component is rejected"),
		Shift->FileDailyReport(nullptr));
	TestFalse(
		TEXT("Component without a work order is rejected"),
		Shift->FileDailyReport(WorkOrders));

	WorkOrders->AssignWorkOrder(
		TEXT("RepairTrailSign"),
		FText::FromString(TEXT("Repair Trail Sign")),
		FText::FromString(TEXT("Inspect and repair the damaged trail sign.")));
	TestFalse(
		TEXT("Active work order is rejected"),
		Shift->FileDailyReport(WorkOrders));

	WorkOrders->CompleteCurrentWorkOrder();
	const FBlackwoodWorkOrder CompletedBeforeFiling =
		WorkOrders->GetCurrentWorkOrder();
	TestTrue(
		TEXT("Completed work order allows report filing"),
		Shift->FileDailyReport(WorkOrders));
	TestEqual(
		TEXT("Filing changes state to ReportFiled"),
		Shift->GetShiftState(),
		EBlackwoodShiftState::ReportFiled);
	TestFalse(TEXT("Filed shift is no longer OnDuty"), Shift->IsOnDuty());
	TestTrue(TEXT("HasFiledReport succeeds after filing"), Shift->HasFiledReport());
	TestFalse(
		TEXT("Filing twice is rejected"),
		Shift->FileDailyReport(WorkOrders));

	const FBlackwoodWorkOrder CompletedAfterFiling =
		WorkOrders->GetCurrentWorkOrder();
	TestEqual(
		TEXT("Filing preserves the work-order ID"),
		CompletedAfterFiling.WorkOrderId,
		CompletedBeforeFiling.WorkOrderId);
	TestEqual(
		TEXT("Filing preserves completed state"),
		CompletedAfterFiling.State,
		EBlackwoodWorkOrderState::Completed);

	const FString FiledSummary = Shift->GetShiftSummary().ToString();
	TestTrue(
		TEXT("Filed summary contains Report Filed"),
		FiledSummary.Contains(TEXT("Report Filed")));
	TestTrue(
		TEXT("Filed summary contains return instruction"),
		FiledSummary.Contains(TEXT("Return to your quarters")));

	return true;
}

#endif
