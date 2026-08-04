#include "Repair/BlackwoodRepairableTrailSign.h"

#include "Components/StaticMeshComponent.h"
#include "Inventory/BlackwoodInventoryComponent.h"
#include "Items/BlackwoodItemDefinition.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Tasks/BlackwoodWorkOrderComponent.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	const FName RepairTrailSignWorkOrderId(TEXT("RepairTrailSign"));
	const FRotator BrokenBoardRotation(0.0, 0.0, 25.0);
	const FRotator RepairedBoardRotation = FRotator::ZeroRotator;
}

ABlackwoodRepairableTrailSign::ABlackwoodRepairableTrailSign()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	SignPost = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SignPost"));
	SetRootComponent(SignPost);
	SignPost->SetSimulatePhysics(false);
	SignPost->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SignPost->SetCollisionResponseToAllChannels(ECR_Block);
	SignPost->SetRelativeScale3D(FVector(0.12, 0.12, 1.4));

	SignBoard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SignBoard"));
	SignBoard->SetupAttachment(SignPost);
	SignBoard->SetSimulatePhysics(false);
	SignBoard->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SignBoard->SetCollisionResponseToAllChannels(ECR_Block);
	SignBoard->SetRelativeLocation(FVector(0.0, 0.0, 130.0));
	SignBoard->SetRelativeScale3D(FVector(0.8, 0.08, 0.28));
	SignBoard->SetRelativeRotation(BrokenBoardRotation);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		SignPost->SetStaticMesh(CubeMesh.Object);
		SignBoard->SetStaticMesh(CubeMesh.Object);
	}
}

bool ABlackwoodRepairableTrailSign::IsRepaired() const
{
	return bIsRepaired;
}

FText ABlackwoodRepairableTrailSign::GetInteractionDisplayName_Implementation() const
{
	return NSLOCTEXT(
		"BlackwoodRepair",
		"TrailSignInteractionName",
		"Trail Sign");
}

void ABlackwoodRepairableTrailSign::Interact_Implementation(AActor* InteractingActor)
{
	if (bIsRepaired)
	{
		PrintRepairMessage(TEXT("Trail Sign Already Repaired"));
		return;
	}

	if (!IsValid(InteractingActor))
	{
		return;
	}

	UBlackwoodWorkOrderComponent* WorkOrders =
		InteractingActor->FindComponentByClass<UBlackwoodWorkOrderComponent>();
	if (!IsValid(WorkOrders)
		|| !WorkOrders->HasActiveWorkOrder()
		|| !WorkOrders->IsCurrentWorkOrder(RepairTrailSignWorkOrderId))
	{
		PrintRepairMessage(TEXT("No Active Repair Work Order"));
		return;
	}

	UBlackwoodInventoryComponent* Inventory =
		InteractingActor->FindComponentByClass<UBlackwoodInventoryComponent>();
	if (!IsValid(Inventory)
		|| !IsValid(RepairKitDefinition)
		|| !Inventory->HasItem(RepairKitDefinition, 1))
	{
		PrintRepairMessage(TEXT("Repair Kit Required"));
		return;
	}

	if (Inventory->RemoveItem(RepairKitDefinition, 1) != 1)
	{
		PrintRepairMessage(TEXT("Repair Kit Required"));
		return;
	}

	SetRepaired(true);
	WorkOrders->CompleteCurrentWorkOrder();
	PrintRepairMessage(TEXT("Trail Sign Repaired"));
}

void ABlackwoodRepairableTrailSign::SetRepaired(const bool bRepaired)
{
	bIsRepaired = bRepaired;
	SignBoard->SetRelativeRotation(
		bIsRepaired ? RepairedBoardRotation : BrokenBoardRotation);
}

void ABlackwoodRepairableTrailSign::PrintRepairMessage(const FString& Message) const
{
	UKismetSystemLibrary::PrintString(
		this,
		Message,
		true,
		true,
		FLinearColor(0.0f, 0.66f, 1.0f),
		2.0);
}
