#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Damaged.generated.h"


UCLASS()
class U2505_98_API UCBTTaskNode_Damaged : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Damaged();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
