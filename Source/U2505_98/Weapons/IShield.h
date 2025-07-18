#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IShield.generated.h"

UINTERFACE(MinimalAPI)
class UIShield : public UInterface
{
	GENERATED_BODY()
};

class U2505_98_API IIShield
{
	GENERATED_BODY()

public:
	virtual bool Do_shield() = 0;
	virtual void Begin_shielded() = 0;
	virtual void End_shielded() = 0;
};
