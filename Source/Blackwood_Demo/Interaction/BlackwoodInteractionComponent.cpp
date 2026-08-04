#include "Interaction/BlackwoodInteractionComponent.h"

#include "Blackwood_Demo.h"
#include "Interaction/BlackwoodInteractable.h"
#include "TimerManager.h"

UBlackwoodInteractionComponent::UBlackwoodInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBlackwoodInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		FocusRefreshTimer,
		this,
		&UBlackwoodInteractionComponent::RefreshFocusedInteractable,
		0.1f,
		true);
}

void UBlackwoodInteractionComponent::EndPlay(
	const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(FocusRefreshTimer);
	}

	SetFocusedInteractable(nullptr);
	Super::EndPlay(EndPlayReason);
}

bool UBlackwoodInteractionComponent::TryInteract()
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		UE_LOG(LogBlackwood_Demo, Warning, TEXT("BlackwoodInteractionComponent has no valid owner."));
		return false;
	}

	SetFocusedInteractable(TraceForInteractable());
	if (!IsValid(CurrentInteractable))
	{
		return false;
	}

	IBlackwoodInteractable::Execute_Interact(CurrentInteractable, Owner);
	return true;
}

AActor* UBlackwoodInteractionComponent::TraceForInteractable() const
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner) || !IsValid(GetWorld()))
	{
		return nullptr;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	Owner->GetActorEyesViewPoint(ViewLocation, ViewRotation);

	const FVector TraceEnd = ViewLocation + (ViewRotation.Vector() * InteractionDistance);
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(BlackwoodInteractionTrace), false, Owner);

	FHitResult Hit;
	if (!GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, TraceEnd, ECC_Visibility, QueryParams))
	{
		return nullptr;
	}

	AActor* HitActor = Hit.GetActor();
	if (!IsValid(HitActor) || !HitActor->GetClass()->ImplementsInterface(UBlackwoodInteractable::StaticClass()))
	{
		return nullptr;
	}

	return HitActor;
}

void UBlackwoodInteractionComponent::RefreshFocusedInteractable()
{
	SetFocusedInteractable(TraceForInteractable());
}

void UBlackwoodInteractionComponent::SetFocusedInteractable(
	AActor* NewInteractable)
{
	if (CurrentInteractable == NewInteractable)
	{
		return;
	}

	CurrentInteractable = NewInteractable;
	if (!IsValid(CurrentInteractable))
	{
		OnFocusedInteractableChanged.Broadcast(FText::GetEmpty(), false);
		return;
	}

	FText DisplayName =
		IBlackwoodInteractable::Execute_GetInteractionDisplayName(CurrentInteractable);
	if (DisplayName.IsEmpty())
	{
		DisplayName =
			NSLOCTEXT("BlackwoodInteraction", "FallbackInteractionName", "Interact");
	}

	OnFocusedInteractableChanged.Broadcast(DisplayName, true);
}
