#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/BlackwoodInteractable.h"
#include "BlackwoodWorldItem.generated.h"

class UBlackwoodItemDefinition;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class BLACKWOOD_DEMO_API ABlackwoodWorldItem : public AActor, public IBlackwoodInteractable
{
	GENERATED_BODY()

public:
	ABlackwoodWorldItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Item")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World Item")
	TObjectPtr<UBlackwoodItemDefinition> ItemDefinition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World Item", meta = (ClampMin = "1", UIMin = "1"))
	int32 Quantity = 1;

	virtual void Interact_Implementation(AActor* InteractingActor) override;
	virtual void PostLoad() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
