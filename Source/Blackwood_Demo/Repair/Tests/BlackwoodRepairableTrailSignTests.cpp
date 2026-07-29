#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Inventory/BlackwoodInventoryComponent.h"
#include "Items/BlackwoodItemDefinition.h"
#include "Repair/BlackwoodRepairableTrailSign.h"
#include "Tasks/BlackwoodWorkOrderComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FBlackwoodRepairableTrailSignTest,
	"Blackwood.Repair.TrailSign.FirstPlayableTask",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FBlackwoodRepairableTrailSignTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	if (!TestNotNull(TEXT("Transient test world is created"), World))
	{
		return false;
	}

	FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
	WorldContext.SetCurrentWorld(World);

	UBlackwoodItemDefinition* RepairKit = NewObject<UBlackwoodItemDefinition>();
	RepairKit->ItemId = TEXT("RepairKit");
	RepairKit->DisplayName = FText::FromString(TEXT("Repair Kit"));
	RepairKit->MaxStackSize = 5;

	AActor* Player = World->SpawnActor<AActor>();
	UBlackwoodInventoryComponent* Inventory =
		NewObject<UBlackwoodInventoryComponent>(Player, TEXT("Inventory"));
	Player->AddInstanceComponent(Inventory);
	Inventory->RegisterComponent();

	UBlackwoodWorkOrderComponent* WorkOrders =
		NewObject<UBlackwoodWorkOrderComponent>(Player, TEXT("WorkOrders"));
	Player->AddInstanceComponent(WorkOrders);
	WorkOrders->RegisterComponent();

	ABlackwoodRepairableTrailSign* Sign =
		World->SpawnActor<ABlackwoodRepairableTrailSign>();
	Sign->RepairKitDefinition = RepairKit;

	Inventory->AddItem(RepairKit, 1);
	Sign->Interact_Implementation(Player);
	TestFalse(TEXT("Cannot repair without active work order"), Sign->IsRepaired());
	TestEqual(
		TEXT("Failed repair does not consume kit"),
		Inventory->GetItemQuantity(RepairKit),
		1);

	WorkOrders->AssignWorkOrder(
		TEXT("RepairTrailSign"),
		FText::FromString(TEXT("Repair Trail Sign")),
		FText::FromString(TEXT("Inspect and repair the damaged trail sign.")));
	Inventory->RemoveItem(RepairKit, 1);
	Sign->Interact_Implementation(Player);
	TestFalse(TEXT("Cannot repair without Repair Kit"), Sign->IsRepaired());
	TestTrue(TEXT("Work order remains active after failed repair"), WorkOrders->HasActiveWorkOrder());

	Inventory->AddItem(RepairKit, 2);
	Sign->Interact_Implementation(Player);
	TestTrue(TEXT("Sign becomes repaired"), Sign->IsRepaired());
	TestEqual(
		TEXT("Repair consumes exactly one Repair Kit"),
		Inventory->GetItemQuantity(RepairKit),
		1);
	TestEqual(
		TEXT("Work order becomes completed"),
		WorkOrders->GetCurrentWorkOrder().State,
		EBlackwoodWorkOrderState::Completed);

	Sign->Interact_Implementation(Player);
	TestTrue(TEXT("Repaired state persists for the session"), Sign->IsRepaired());
	TestEqual(
		TEXT("Repaired sign cannot consume another kit"),
		Inventory->GetItemQuantity(RepairKit),
		1);
	TestEqual(
		TEXT("Completed work order remains completed"),
		WorkOrders->GetCurrentWorkOrder().State,
		EBlackwoodWorkOrderState::Completed);

	GEngine->DestroyWorldContext(World);
	World->DestroyWorld(false);
	return true;
}

#endif
