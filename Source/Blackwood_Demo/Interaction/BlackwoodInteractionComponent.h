#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlackwoodInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FBlackwoodFocusedInteractableChanged,
	FText,
	DisplayName,
	bool,
	bVisible);

UCLASS(ClassGroup = (Blackwood), meta = (BlueprintSpawnableComponent))
class BLACKWOOD_DEMO_API UBlackwoodInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBlackwoodInteractionComponent();

	UFUNCTION(BlueprintCallable, Category = "Blackwood|Interaction")
	bool TryInteract();

	UPROPERTY(BlueprintAssignable, Category = "Blackwood|Interaction")
	FBlackwoodFocusedInteractableChanged OnFocusedInteractableChanged;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Blackwood|Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> CurrentInteractable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackwood|Interaction", meta = (ClampMin = "0.0", Units = "cm"))
	float InteractionDistance = 300.0f;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	AActor* TraceForInteractable() const;
	void RefreshFocusedInteractable();
	void SetFocusedInteractable(AActor* NewInteractable);

	FTimerHandle FocusRefreshTimer;
};
