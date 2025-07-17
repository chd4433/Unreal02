#include "BehaviorTree/CBTTaskNode_Attack.h"
#include "Global.h"

#include "Chracters/CAIController.h"
#include "Chracters/CEnemy_AI.h"
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

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	IISword* sword = Cast<IISword>(ai);

	sword->End_DoAction();

	return EBTNodeResult::Aborted;
}
