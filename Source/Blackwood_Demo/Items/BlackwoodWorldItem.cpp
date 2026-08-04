#include "Items/BlackwoodWorldItem.h"

#include "Components/StaticMeshComponent.h"
#include "Inventory/BlackwoodInventoryComponent.h"
#include "Items/BlackwoodItemDefinition.h"

ABlackwoodWorldItem::ABlackwoodWorldItem()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	SetRootComponent(ItemMesh);
	ItemMesh->SetSimulatePhysics(false);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void ABlackwoodWorldItem::Interact_Implementation(AActor* InteractingActor)
{
	if (!IsValid(InteractingActor) || !IsValid(ItemDefinition) || Quantity < 1)
	{
		return;
	}

	UBlackwoodInventoryComponent* Inventory =
		InteractingActor->FindComponentByClass<UBlackwoodInventoryComponent>();
	if (!IsValid(Inventory))
	{
		return;
	}

	const int32 AddedQuantity = Inventory->AddItem(ItemDefinition, Quantity);
	if (AddedQuantity < 1)
	{
		return;
	}

	Quantity = FMath::Max(0, Quantity - AddedQuantity);
	if (Quantity == 0)
	{
		Destroy();
	}
}

FText ABlackwoodWorldItem::GetInteractionDisplayName_Implementation() const
{
	return InteractionDisplayName;
}

void ABlackwoodWorldItem::PostLoad()
{
	Super::PostLoad();
	Quantity = FMath::Max(1, Quantity);
}

#if WITH_EDITOR
void ABlackwoodWorldItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Quantity = FMath::Max(1, Quantity);
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
