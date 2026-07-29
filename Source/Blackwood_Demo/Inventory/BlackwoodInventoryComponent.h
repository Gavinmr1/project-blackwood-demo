#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/BlackwoodInventoryTypes.h"
#include "BlackwoodInventoryComponent.generated.h"

class UBlackwoodItemDefinition;

UCLASS(ClassGroup = (Blackwood), meta = (BlueprintSpawnableComponent))
class BLACKWOOD_DEMO_API UBlackwoodInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBlackwoodInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Blackwood|Inventory")
	int32 AddItem(UBlackwoodItemDefinition* ItemDefinition, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Blackwood|Inventory")
	int32 RemoveItem(UBlackwoodItemDefinition* ItemDefinition, int32 Quantity);

	UFUNCTION(BlueprintPure, Category = "Blackwood|Inventory")
	int32 GetItemQuantity(UBlackwoodItemDefinition* ItemDefinition) const;

	UFUNCTION(BlueprintPure, Category = "Blackwood|Inventory")
	bool HasItem(UBlackwoodItemDefinition* ItemDefinition, int32 RequiredQuantity = 1) const;

	UFUNCTION(BlueprintPure, Category = "Blackwood|Inventory")
	const TArray<FBlackwoodInventoryEntry>& GetInventory() const;

	UFUNCTION(BlueprintPure, Category = "Blackwood|Inventory")
	FText GetInventorySummary() const;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<FBlackwoodInventoryEntry> Inventory;
};
