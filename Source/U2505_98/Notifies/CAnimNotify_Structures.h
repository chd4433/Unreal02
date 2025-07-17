
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CAnimNotify_Structures.generated.h"


UENUM()
enum class EAnimNotify_Flow : uint8
{
	Begin = 0, End, Max,
};

UCLASS()
class U2505_98_API UCAnimNotify_Structures : public UObject
{
	GENERATED_BODY()

public:
	static FColor GetNotifyColor(EAnimNotify_Flow InValue);
	static FString GetNotifyName(FString InName, EAnimNotify_Flow InValue);
	
};
