#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Day/BlackwoodShiftTypes.h"
#include "BlackwoodShiftComponent.generated.h"

class UBlackwoodWorkOrderComponent;

UCLASS(ClassGroup = (Blackwood), meta = (BlueprintSpawnableComponent))
class BLACKWOOD_DEMO_API UBlackwoodShiftComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBlackwoodShiftComponent();

	UFUNCTION(BlueprintPure, Category = "Blackwood|Day")
	EBlackwoodShiftState GetShiftState() const;

	UFUNCTION(BlueprintPure, Category = "Blackwood|Day")
	bool IsOnDuty() const;

	UFUNCTION(BlueprintPure, Category = "Blackwood|Day")
	bool HasFiledReport() const;

	UFUNCTION(BlueprintCallable, Category = "Blackwood|Day")
	bool FileDailyReport(UBlackwoodWorkOrderComponent* WorkOrderComponent);

	UFUNCTION(BlueprintPure, Category = "Blackwood|Day")
	FText GetShiftSummary() const;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Shift", meta = (AllowPrivateAccess = "true"))
	EBlackwoodShiftState ShiftState = EBlackwoodShiftState::OnDuty;
};
