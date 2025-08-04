#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_ComboAttack.generated.h"

/**
 * 
 */
UCLASS()
class U2505_98_API UCBTTaskNode_ComboAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_ComboAttack();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	int ComboCount;
	
};
