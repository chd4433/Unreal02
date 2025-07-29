#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IHittedReaction.generated.h"

UINTERFACE(MinimalAPI)
class UIHittedReaction : public UInterface
{
	GENERATED_BODY()
};


class U2505_98_API IIHittedReaction
{
	GENERATED_BODY()

public:
	virtual bool Air_Reaction() = 0;
	virtual void SetAir_Reaction(bool Inbool) = 0;

};
