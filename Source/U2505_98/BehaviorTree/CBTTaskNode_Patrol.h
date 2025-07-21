
#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Patrol.generated.h"


UCLASS()
class U2505_98_API UCBTTaskNode_Patrol : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Common")
	bool bDebug;

	UPROPERTY(EditAnywhere, Category = "Common")
	FName PatrolKey = "Patrol";

	UPROPERTY(EditAnywhere, Category = "Common")
	float AcceptanceRadius = 20;

private:
	UPROPERTY(EditAnywhere, Category = "WithoutPath")
	float PatrolRadius = 1000.0f;

public:
	UCBTTaskNode_Patrol();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
