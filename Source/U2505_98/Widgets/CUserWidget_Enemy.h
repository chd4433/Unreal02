#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Enemy.generated.h"


UCLASS()
class U2505_98_API UCUserWidget_Enemy : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateControllerName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHealth(float InHealth, float InMaxHealth);
};
