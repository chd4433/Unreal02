
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ISword.generated.h"

UINTERFACE(MinimalAPI)
class UISword : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class U2505_98_API IISword
{
	GENERATED_BODY()

public:
	virtual void OnSword() = 0;
	virtual void UpperAction() = 0;

public:
	virtual bool IsEquipped() = 0;
	virtual bool IsEquipping() = 0;
	virtual bool IsAttacking() = 0;
	virtual bool IsAttached_Hand() = 0;

	virtual void Begin_Equip() = 0;
	virtual void End_Equip() = 0;

	virtual void OnDoAction() = 0;
	virtual void Begin_DoAction() = 0;
	virtual void End_DoAction() = 0;

	virtual void Begin_Combo() = 0;
	virtual void End_Combo() = 0;

	virtual void Begin_Collision() = 0;
	virtual void End_Collision() = 0;

	virtual void Destroy_Sword() = 0;
};
