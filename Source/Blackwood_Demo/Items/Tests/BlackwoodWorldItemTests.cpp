#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Inventory/BlackwoodInventoryComponent.h"
#include "Items/BlackwoodItemDefinition.h"
#include "Items/BlackwoodWorldItem.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FBlackwoodWorldItemTest,
	"Blackwood.Items.WorldItem.Transfer",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FBlackwoodWorldItemTest::RunTest(const FString& Parameters)
{
	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	if (!TestNotNull(TEXT("Transient test world is created"), World))
	{
		return false;
	}

	FWorldContext& WorldContext = GEngine->CreateNewWorldContext(EWorldType::Game);
	WorldContext.SetCurrentWorld(World);

	UBlackwoodItemDefinition* Item = NewObject<UBlackwoodItemDefinition>();
	Item->ItemId = TEXT("ScrapWood");
	Item->DisplayName = FText::FromString(TEXT("Scrap Wood"));
	Item->MaxStackSize = 10;

	AActor* InventoryOwner = World->SpawnActor<AActor>();
	UBlackwoodInventoryComponent* Inventory =
		NewObject<UBlackwoodInventoryComponent>(InventoryOwner, TEXT("Inventory"));
	InventoryOwner->AddInstanceComponent(Inventory);
	Inventory->RegisterComponent();

	AActor* ActorWithoutInventory = World->SpawnActor<AActor>();

	ABlackwoodWorldItem* InvalidActorPickup = World->SpawnActor<ABlackwoodWorldItem>();
	InvalidActorPickup->ItemDefinition = Item;
	InvalidActorPickup->Quantity = 3;
	InvalidActorPickup->Interact_Implementation(nullptr);
	TestEqual(TEXT("Invalid interacting actor changes nothing"), InvalidActorPickup->Quantity, 3);

	InvalidActorPickup->Interact_Implementation(ActorWithoutInventory);
	TestEqual(TEXT("Actor without inventory changes nothing"), InvalidActorPickup->Quantity, 3);

	ABlackwoodWorldItem* NullDefinitionPickup = World->SpawnActor<ABlackwoodWorldItem>();
	NullDefinitionPickup->Quantity = 3;
	NullDefinitionPickup->Interact_Implementation(InventoryOwner);
	TestEqual(TEXT("Null item definition changes nothing"), NullDefinitionPickup->Quantity, 3);

	ABlackwoodWorldItem* FullTransferPickup = World->SpawnActor<ABlackwoodWorldItem>();
	FullTransferPickup->ItemDefinition = Item;
	FullTransferPickup->Quantity = 3;
	FullTransferPickup->Interact_Implementation(InventoryOwner);
	TestEqual(TEXT("Valid pickup transfers its full quantity"), Inventory->GetItemQuantity(Item), 3);
	TestEqual(TEXT("Full transfer reduces remaining quantity to zero"), FullTransferPickup->Quantity, 0);

	ABlackwoodWorldItem* PartialTransferPickup = World->SpawnActor<ABlackwoodWorldItem>();
	PartialTransferPickup->ItemDefinition = Item;
	PartialTransferPickup->Quantity = 8;
	PartialTransferPickup->Interact_Implementation(InventoryOwner);
	TestEqual(TEXT("Partial transfer fills the stack"), Inventory->GetItemQuantity(Item), 10);
	TestEqual(TEXT("Partial transfer leaves only the unaccepted quantity"), PartialTransferPickup->Quantity, 1);

	PartialTransferPickup->Interact_Implementation(InventoryOwner);
	TestEqual(TEXT("Full inventory remains capped"), Inventory->GetItemQuantity(Item), 10);
	TestEqual(TEXT("Full inventory leaves pickup unchanged"), PartialTransferPickup->Quantity, 1);
	TestTrue(TEXT("Pickup quantity never becomes negative"), PartialTransferPickup->Quantity >= 0);
	TestEqual(TEXT("Summary uses display name and quantity"), Inventory->GetInventorySummary().ToString(), TEXT("Scrap Wood: 10"));

	GEngine->DestroyWorldContext(World);
	World->DestroyWorld(false);
	return true;
}

#endif
