#include "BehaviorTree/CBTService_AI.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Chracters/CAIController.h"
#include "Chracters/CEnemy_AI.h"
#include "Chracters/CAIStructures.h"
#include "Weapons/ISword.h"
#include "Chracters/CAIGroup.h"

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

	IISword* sword = Cast<IISword>(ai);
	CheckNull(sword);

	ACAIController* ai_Controller = Cast<ACAIController>(ai->GetController());
	if (!!ai_Controller)
		ai_Controller->ClearFocus(EAIFocusPriority::Gameplay);
	

	if (!!(ai->GetAiGroupManager()))
	{
		ACharacter* groupTarget = Cast<ACharacter>(blackboard->GetValueAsObject(GroupTargetKey));

		if (!!groupTarget)
		{
			float distance = ai->GetDistanceTo(groupTarget);
			if (distance > ActionDistance)
			{
				if (sword->IsAttacking() == false)
				{
					blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::GroupTargetApproach);
				}

				return;
			}
			else
			{
				switch (ai->GetActionState())
				{
				case Action_State::Attack:
					if (sword->IsAttacking() == false)
						blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Attack);
					return;
				case Action_State::Shield:
					blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Shield);
					return;
				case Action_State::ComboAttack:
					blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::ComboAttack);
					return;
				}
			}
		}

		switch (ai->GetAiGroupManager()->GetState())
		{
		case Group_State::PlayerFighting:
		{
			ACharacter* wanderTarget = Cast<ACharacter>(blackboard->GetValueAsObject(WanderTargetKey));
			if (!!wanderTarget)
			{
				ACAIController* aiController = Cast<ACAIController>(ai->GetController());
				if(!!aiController)
					aiController->SetFocus(wanderTarget);
			}

			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Wander);
		}

			return;
		case Group_State::GroupFighting:
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Partrol);
			return;
		default:
			break;
		}
	}

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


	float distance = ai->GetDistanceTo(target);

	if (distance > ActionDistance)
	{
		if (sword->IsAttacking() == false)
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Approach);

		return;
	}
	else
	{
		switch (ai->GetActionState())
		{
		case Action_State::Attack:
			if (sword->IsAttacking() == false)
				blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Attack);
			return;
		case Action_State::Shield:
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::Shield);
		case Action_State::ComboAttack:
			blackboard->SetValueAsEnum(AIStateKey, (uint8)EAIStateType::ComboAttack);
			return;
		}
	}
}
