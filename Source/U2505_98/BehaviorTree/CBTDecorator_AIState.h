#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Chracters/CAIStructures.h"
#include "CBTDecorator_AIState.generated.h"


UCLASS()
class U2505_98_API UCBTDecorator_AIState : public UBTDecorator
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Type")
	EAIStateType State;

private:
	FBlackboardKeySelector key;

public:
	UCBTDecorator_AIState();

protected:
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
private:
	EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

private:
	void InitializeFromAsset(UBehaviorTree& Asset) override;



#if WITH_EDITOR
	void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
