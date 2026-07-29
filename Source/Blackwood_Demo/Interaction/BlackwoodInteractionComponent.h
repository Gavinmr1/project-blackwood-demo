#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlackwoodInteractionComponent.generated.h"

UCLASS(ClassGroup = (Blackwood), meta = (BlueprintSpawnableComponent))
class BLACKWOOD_DEMO_API UBlackwoodInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBlackwoodInteractionComponent();

	UFUNCTION(BlueprintCallable, Category = "Blackwood|Interaction")
	bool TryInteract();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackwood|Interaction", meta = (ClampMin = "0.0", Units = "cm"))
	float InteractionDistance = 300.0f;
};
