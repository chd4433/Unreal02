#include "BehaviorTree/CBTDecorator_AIState.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBTDecorator_AIState::UCBTDecorator_AIState()
{
	NodeName = StaticEnum<EAIStateType>()->GetNameStringByValue((int64)State);;

	FlowAbortMode = EBTFlowAbortMode::Self;
	key.SelectedKeyName = "AIState";

	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
}

void UCBTDecorator_AIState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	if (!!blackboard)
	{
		FBlackboard::FKey fkey = key.GetSelectedKeyID();
		blackboard->RegisterObserver(fkey, this, FOnBlackboardChangeNotification::CreateUObject(this, &UCBTDecorator_AIState::OnBlackboardKeyValueChange));
	}
}

void UCBTDecorator_AIState::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	if (!!blackboard)
	{
		blackboard->UnregisterObserversFrom(this);
	}
}

bool UCBTDecorator_AIState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();

	EAIStateType type = (EAIStateType)blackboard->GetValueAsEnum("AIState");


	return !!blackboard && State == type;
}

EBlackboardNotificationResult UCBTDecorator_AIState::OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* behaviorTree = Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent());

	if (behaviorTree == nullptr)
		return EBlackboardNotificationResult::RemoveObserver;


	if (key.GetSelectedKeyID() == ChangedKeyID)
		ConditionalFlowAbort(*behaviorTree, EBTDecoratorAbortRequest::ConditionResultChanged);


	return EBlackboardNotificationResult::ContinueObserving;
}

void UCBTDecorator_AIState::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* asset = GetBlackboardAsset();
	if (ensure(asset))
	{
		key.ResolveSelectedKey(*asset);
	}
}

#if WITH_EDITOR
void UCBTDecorator_AIState::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName name = PropertyChangedEvent.Property->GetFName();
	if (name == GET_MEMBER_NAME_CHECKED(UCBTDecorator_AIState, State))
	{

		FLog::Log(StaticEnum<EAIStateType>()->GetNameStringByValue((int64)State));
		NodeName = StaticEnum<EAIStateType>()->GetNameStringByValue((int64)State);
	}
}
#endif