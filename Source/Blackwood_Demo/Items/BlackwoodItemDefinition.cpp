#include "Items/BlackwoodItemDefinition.h"

void UBlackwoodItemDefinition::PostLoad()
{
	Super::PostLoad();
	MaxStackSize = FMath::Max(1, MaxStackSize);
}

#if WITH_EDITOR
void UBlackwoodItemDefinition::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	MaxStackSize = FMath::Max(1, MaxStackSize);
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif
