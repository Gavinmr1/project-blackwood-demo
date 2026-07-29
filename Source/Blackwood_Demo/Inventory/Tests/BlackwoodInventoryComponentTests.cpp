#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Inventory/BlackwoodInventoryComponent.h"
#include "Items/BlackwoodItemDefinition.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FBlackwoodInventoryComponentTest,
	"Blackwood.Inventory.Component.Foundation",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FBlackwoodInventoryComponentTest::RunTest(const FString& Parameters)
{
	UBlackwoodInventoryComponent* Inventory = NewObject<UBlackwoodInventoryComponent>();
	UBlackwoodItemDefinition* Item = NewObject<UBlackwoodItemDefinition>();
	Item->ItemId = TEXT("ScrapWood");
	Item->MaxStackSize = 10;

	TestEqual(TEXT("Null item is rejected"), Inventory->AddItem(nullptr, 1), 0);
	TestEqual(TEXT("Zero add is rejected"), Inventory->AddItem(Item, 0), 0);
	TestEqual(TEXT("Negative add is rejected"), Inventory->AddItem(Item, -1), 0);
	TestEqual(TEXT("One item is added"), Inventory->AddItem(Item, 1), 1);
	TestEqual(TEXT("Multiple items are added"), Inventory->AddItem(Item, 4), 4);
	TestEqual(TEXT("Quantity query returns the total"), Inventory->GetItemQuantity(Item), 5);
	TestTrue(TEXT("HasItem succeeds"), Inventory->HasItem(Item, 5));
	TestFalse(TEXT("HasItem fails for insufficient quantity"), Inventory->HasItem(Item, 6));
	TestFalse(TEXT("HasItem rejects zero required quantity"), Inventory->HasItem(Item, 0));
	TestEqual(TEXT("Partial removal succeeds"), Inventory->RemoveItem(Item, 2), 2);
	TestEqual(TEXT("Partial removal leaves the correct quantity"), Inventory->GetItemQuantity(Item), 3);
	TestEqual(TEXT("Oversized removal removes only the available quantity"), Inventory->RemoveItem(Item, 100), 3);
	TestEqual(TEXT("Entry is absent at zero"), Inventory->GetItemQuantity(Item), 0);
	TestEqual(TEXT("Repeated removal cannot become negative"), Inventory->RemoveItem(Item, 1), 0);
	TestEqual(TEXT("Zero removal is rejected"), Inventory->RemoveItem(Item, 0), 0);
	TestEqual(TEXT("Negative removal is rejected"), Inventory->RemoveItem(Item, -1), 0);
	TestEqual(TEXT("Add above maximum accepts only ten"), Inventory->AddItem(Item, 15), 10);
	TestEqual(TEXT("Maximum stack size is respected"), Inventory->GetItemQuantity(Item), 10);
	TestEqual(TEXT("Full stack accepts no additional items"), Inventory->AddItem(Item, 1), 0);
	TestEqual(TEXT("Inventory contains one merged entry"), Inventory->GetInventory().Num(), 1);

	return true;
}

#endif
