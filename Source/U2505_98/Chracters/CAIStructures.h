
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CAIStructures.generated.h"

UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0, Partrol, Equip, Approach,GroupTargetApproach, Attack, Damaged, GoToLocation, Max,
};

UCLASS()
class U2505_98_API UCAIStructures : public UObject
{
	GENERATED_BODY()
	
};
