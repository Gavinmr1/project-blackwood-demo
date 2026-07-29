#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BlackwoodInteractable.generated.h"

UINTERFACE(Blueprintable, BlueprintType)
class BLACKWOOD_DEMO_API UBlackwoodInteractable : public UInterface
{
	GENERATED_BODY()
};

class BLACKWOOD_DEMO_API IBlackwoodInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Blackwood|Interaction")
	void Interact(AActor* InteractingActor);
};
