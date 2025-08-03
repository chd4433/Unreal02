#include "BehaviorTree/CBTTaskNode_Shield.h"
#include "Global.h"

#include "Chracters/CAIController.h"
#include "Chracters/CEnemy_AI.h"
#include "Weapons/IShield.h"

UCBTTaskNode_Shield::UCBTTaskNode_Shield()
{
	NodeName = "Shield";

	ShieldTime = 3.0f;

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Shield::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	IIShield* shield = Cast<IIShield>(ai);
	CheckNullResult(shield, EBTNodeResult::Failed);

	controller->StopMovement();
	shield->Begin_shielded();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Shield::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (ShieldTime >= 0.0f)
	{
		ShieldTime -= DeltaSeconds;
	}
	else
	{
		ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
		CheckNull(controller);

		ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
		CheckNull(ai);

		IIShield* shield = Cast<IIShield>(ai);
		CheckNull(shield);

		ai->SetActionState();
		shield->End_shielded();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

EBTNodeResult::Type UCBTTaskNode_Shield::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	IIShield* shield = Cast<IIShield>(ai);
	CheckNullResult(shield, EBTNodeResult::Failed);

	ai->SetActionState();
	shield->End_shielded();

	return EBTNodeResult::Aborted;
}
