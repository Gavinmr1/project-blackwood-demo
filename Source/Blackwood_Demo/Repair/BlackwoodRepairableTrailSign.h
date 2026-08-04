#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/BlackwoodInteractable.h"
#include "BlackwoodRepairableTrailSign.generated.h"

class UBlackwoodItemDefinition;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class BLACKWOOD_DEMO_API ABlackwoodRepairableTrailSign
	: public AActor
	, public IBlackwoodInteractable
{
	GENERATED_BODY()

public:
	ABlackwoodRepairableTrailSign();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trail Sign")
	TObjectPtr<UStaticMeshComponent> SignPost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trail Sign")
	TObjectPtr<UStaticMeshComponent> SignBoard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trail Sign")
	TObjectPtr<UBlackwoodItemDefinition> RepairKitDefinition;

	UFUNCTION(BlueprintPure, Category = "Blackwood|Repair")
	bool IsRepaired() const;

	virtual void Interact_Implementation(AActor* InteractingActor) override;
	virtual FText GetInteractionDisplayName_Implementation() const override;

private:
	void SetRepaired(bool bRepaired);
	void PrintRepairMessage(const FString& Message) const;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Trail Sign", meta = (AllowPrivateAccess = "true"))
	bool bIsRepaired = false;
};
