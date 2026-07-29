#include "Interaction/BlackwoodInteractionComponent.h"

#include "Blackwood_Demo.h"
#include "Interaction/BlackwoodInteractable.h"

UBlackwoodInteractionComponent::UBlackwoodInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UBlackwoodInteractionComponent::TryInteract()
{
	AActor* Owner = GetOwner();
	if (!IsValid(Owner))
	{
		UE_LOG(LogBlackwood_Demo, Warning, TEXT("BlackwoodInteractionComponent has no valid owner."));
		return false;
	}

	FVector ViewLocation;
	FRotator ViewRotation;
	Owner->GetActorEyesViewPoint(ViewLocation, ViewRotation);

	const FVector TraceEnd = ViewLocation + (ViewRotation.Vector() * InteractionDistance);
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(BlackwoodInteractionTrace), false, Owner);

	FHitResult Hit;
	if (!GetWorld()->LineTraceSingleByChannel(Hit, ViewLocation, TraceEnd, ECC_Visibility, QueryParams))
	{
		return false;
	}

	AActor* HitActor = Hit.GetActor();
	if (!IsValid(HitActor) || !HitActor->GetClass()->ImplementsInterface(UBlackwoodInteractable::StaticClass()))
	{
		return false;
	}

	IBlackwoodInteractable::Execute_Interact(HitActor, Owner);
	return true;
}
