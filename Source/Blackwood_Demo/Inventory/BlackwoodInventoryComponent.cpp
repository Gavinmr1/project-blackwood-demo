#include "Inventory/BlackwoodInventoryComponent.h"

#include "Items/BlackwoodItemDefinition.h"

UBlackwoodInventoryComponent::UBlackwoodInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

int32 UBlackwoodInventoryComponent::AddItem(UBlackwoodItemDefinition* ItemDefinition, const int32 Quantity)
{
	if (!IsValid(ItemDefinition) || Quantity < 1)
	{
		return 0;
	}

	const int32 MaxStackSize = FMath::Max(1, ItemDefinition->MaxStackSize);
	FBlackwoodInventoryEntry* ExistingEntry = Inventory.FindByPredicate(
		[ItemDefinition](const FBlackwoodInventoryEntry& Entry)
		{
			return Entry.ItemDefinition == ItemDefinition;
		});

	if (ExistingEntry)
	{
		ExistingEntry->Quantity = FMath::Clamp(ExistingEntry->Quantity, 0, MaxStackSize);
		const int32 AddedQuantity = FMath::Min(Quantity, MaxStackSize - ExistingEntry->Quantity);
		ExistingEntry->Quantity += AddedQuantity;
		return AddedQuantity;
	}

	const int32 AddedQuantity = FMath::Min(Quantity, MaxStackSize);
	FBlackwoodInventoryEntry& NewEntry = Inventory.AddDefaulted_GetRef();
	NewEntry.ItemDefinition = ItemDefinition;
	NewEntry.Quantity = AddedQuantity;
	return AddedQuantity;
}

int32 UBlackwoodInventoryComponent::RemoveItem(UBlackwoodItemDefinition* ItemDefinition, const int32 Quantity)
{
	if (!IsValid(ItemDefinition) || Quantity < 1)
	{
		return 0;
	}

	const int32 EntryIndex = Inventory.IndexOfByPredicate(
		[ItemDefinition](const FBlackwoodInventoryEntry& Entry)
		{
			return Entry.ItemDefinition == ItemDefinition;
		});

	if (EntryIndex == INDEX_NONE)
	{
		return 0;
	}

	FBlackwoodInventoryEntry& Entry = Inventory[EntryIndex];
	Entry.Quantity = FMath::Max(0, Entry.Quantity);
	const int32 RemovedQuantity = FMath::Min(Quantity, Entry.Quantity);
	Entry.Quantity -= RemovedQuantity;

	if (Entry.Quantity == 0)
	{
		Inventory.RemoveAt(EntryIndex);
	}

	return RemovedQuantity;
}

int32 UBlackwoodInventoryComponent::GetItemQuantity(UBlackwoodItemDefinition* ItemDefinition) const
{
	if (!IsValid(ItemDefinition))
	{
		return 0;
	}

	const FBlackwoodInventoryEntry* Entry = Inventory.FindByPredicate(
		[ItemDefinition](const FBlackwoodInventoryEntry& InventoryEntry)
		{
			return InventoryEntry.ItemDefinition == ItemDefinition;
		});

	return Entry ? FMath::Max(0, Entry->Quantity) : 0;
}

bool UBlackwoodInventoryComponent::HasItem(
	UBlackwoodItemDefinition* ItemDefinition,
	const int32 RequiredQuantity) const
{
	return IsValid(ItemDefinition)
		&& RequiredQuantity >= 1
		&& GetItemQuantity(ItemDefinition) >= RequiredQuantity;
}

const TArray<FBlackwoodInventoryEntry>& UBlackwoodInventoryComponent::GetInventory() const
{
	return Inventory;
}

FText UBlackwoodInventoryComponent::GetInventorySummary() const
{
	TArray<FString> SummaryLines;

	for (const FBlackwoodInventoryEntry& Entry : Inventory)
	{
		if (IsValid(Entry.ItemDefinition) && Entry.Quantity > 0)
		{
			SummaryLines.Add(FString::Printf(
				TEXT("%s: %d"),
				*Entry.ItemDefinition->DisplayName.ToString(),
				Entry.Quantity));
		}
	}

	return SummaryLines.IsEmpty()
		? FText::FromString(TEXT("Inventory Empty"))
		: FText::FromString(FString::Join(SummaryLines, TEXT("\n")));
}
