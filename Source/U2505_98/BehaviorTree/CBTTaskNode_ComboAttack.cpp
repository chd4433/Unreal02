#include "BehaviorTree/CBTTaskNode_ComboAttack.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Chracters/CAIController.h"
#include "Chracters/CEnemy_AI.h"
#include "Chracters/CAIStructures.h"
#include "Weapons/ISword.h"

UCBTTaskNode_ComboAttack::UCBTTaskNode_ComboAttack()
{
	NodeName = "ComboAttack";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_ComboAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	if (type != EAIStateType::ComboAttack)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	controller->StopMovement();
	sword->OnDoAction();

	ComboCount = 0;

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_ComboAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	IISword* sword = Cast<IISword>(ai);
	CheckNull(sword);

	if (sword->IsAttacking() == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	else
	{
		if (ComboCount < 2)
		{
			sword->OnDoAction();
			if (sword->IsComboAttack())
			{
				ComboCount++;
				sword->SetComboAttack(false);
			}
		}
		else
		{
			ai->SetActionState();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

EBTNodeResult::Type UCBTTaskNode_ComboAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Aborted);


	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Aborted);

	IISword* sword = Cast<IISword>(ai);
	CheckNullResult(sword, EBTNodeResult::Aborted);

	ai->SetActionState();
	sword->End_DoAction();

	return EBTNodeResult::Aborted;
}
