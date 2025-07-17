#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Speed.generated.h"

UENUM()
enum class ESpeedType : uint8
{
	Walk = 0, Run, Sprint, Max,
};

UCLASS()
class U2505_98_API UCBTTaskNode_Speed : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Speed")
	ESpeedType Type;

public:
	UCBTTaskNode_Speed();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
