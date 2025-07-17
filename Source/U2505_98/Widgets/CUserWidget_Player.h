
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Player.generated.h"

UCLASS()
class U2505_98_API UCUserWidget_Player : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEquipped(bool bEquipped);
};
