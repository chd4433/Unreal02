#include "BehaviorTree/CBTService_AI.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Chracters/CAIController.h"
#include "Chracters/CEnemy_AI.h"
#include "Chracters/CAIStructures.h"
#include "Weapons/ISword.h"

UCBTService_AI::UCBTService_AI()
{
	NodeName = "Service";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_AI::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);
	CheckTrue(ai->GetDead());

	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	CheckNull(blackboard);

	ACharacter* target = Cast<ACharacter>(blackboard->GetValueAsObject(TargetKey));


	if (target == nullptr)
	{
		if (bPatrol)
		{
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Partrol);

		}
		else
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Wait);

		return;
	}


	IISword* sword = Cast<IISword>(ai);
	CheckNull(sword);

	//if (sword->IsEquipped() == false)
	//{
	//	blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Equip);

	//	return;
	//}

	float distance = ai->GetDistanceTo(target);

	if (distance > ActionDistance)
	{
		if (sword->IsAttacking() == false)
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Approach);

		return;
	}
	else
	{
		if (sword->IsAttacking() == false)
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Attack);
		
		return;
	}
}
