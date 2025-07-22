#include "BehaviorTree/CBTTaskNode_GoToLocation.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

#include "Chracters/CAIController.h"
#include "Chracters/CEnemy_AI.h"
#include "Chracters/CAIStructures.h"

UCBTTaskNode_GoToLocation::UCBTTaskNode_GoToLocation()
{
	NodeName = "GoToLocation";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_GoToLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);;

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_GoToLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	CheckNull(blackboard);
	FVector Location = blackboard->GetValueAsVector(LocationKey);

	EPathFollowingRequestResult::Type result = controller->MoveToLocation(Location, AcceptanceRadius, false);
	

	switch (result)
	{
	case EPathFollowingRequestResult::Failed:
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		break;

	case EPathFollowingRequestResult::AlreadyAtGoal:
		{
			blackboard->SetValueAsEnum("AIState", (uint8)EAIStateType::Wait);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			break;
		}

	}

}

EBTNodeResult::Type UCBTTaskNode_GoToLocation::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Aborted;
}
