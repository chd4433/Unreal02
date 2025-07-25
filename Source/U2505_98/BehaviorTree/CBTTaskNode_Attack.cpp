#include "BehaviorTree/CBTTaskNode_Attack.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Chracters/CAIController.h"
#include "Chracters/CEnemy_AI.h"
#include "Chracters/CAIStructures.h"
#include "Weapons/ISword.h"

UCBTTaskNode_Attack::UCBTTaskNode_Attack()
{
	NodeName = "Attack";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	IISword* sword = Cast<IISword>(ai);
	CheckNullResult(sword, EBTNodeResult::Failed);
	CheckFalseResult(sword->IsEquipped(), EBTNodeResult::Failed);

	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	CheckNullResult(blackboard, EBTNodeResult::Failed);

	EAIStateType type = (EAIStateType)blackboard->GetValueAsEnum("AIState");
	if (type != EAIStateType::Attack)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	controller->StopMovement();
	sword->OnDoAction();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	IISword* sword = Cast<IISword>(ai);
	CheckNull(sword);

	if (sword->IsAttacking() == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UCBTTaskNode_Attack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Aborted);


	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Aborted);

	IISword* sword = Cast<IISword>(ai);
	CheckNullResult(sword, EBTNodeResult::Aborted);

	sword->End_DoAction();

	return EBTNodeResult::Aborted;
}
