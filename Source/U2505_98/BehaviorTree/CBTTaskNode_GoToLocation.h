#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_GoToLocation.generated.h"

UCLASS()
class U2505_98_API UCBTTaskNode_GoToLocation : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Common")
	FName LocationKey = "GoToLocation";

	UPROPERTY(EditAnywhere, Category = "Common")
	float AcceptanceRadius = 100.f;

public:
	UCBTTaskNode_GoToLocation();
	
private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
