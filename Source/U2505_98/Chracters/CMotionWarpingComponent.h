#pragma once

#include "CoreMinimal.h"
#include "MotionWarpingComponent.h"
#include "CMotionWarpingComponent.generated.h"

UCLASS()
class U2505_98_API UCMotionWarpingComponent : public UMotionWarpingComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Debug")
	bool bDebug;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance = 100;
	
public:
	UCMotionWarpingComponent(const FObjectInitializer& ObjectInitializer);

public:
	void DoMotionWarping();

private:
	void LineTrace();

private:
	class ACharacter* OwnerCharacter;
	AActor* HitActor;
};
