#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Chracters/CAIController.h"
#include "Chracters/CEnemy_AI.h"


UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	CheckNullResult(blackboard, EBTNodeResult::Failed);


	FVector start = ai->GetStartLocation();
	//FVector start = ai->GetActorLocation();

	UNavigationSystemV1* navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	CheckNullResult(navigation, EBTNodeResult::Failed);


	FNavLocation point;

	while (true)
	{
		if (navigation->GetRandomPointInNavigableRadius(start, PatrolRadius, point))
			break;
	}


	blackboard->SetValueAsVector(PatrolKey, point.Location);
	//FLog::Log(point.Location);


	if (bDebug)
	{
		DrawDebugSphere(ai->GetWorld(), point.Location, 25, 40, FColor::Green, false, 5.0f);


		FVector tempStart = FVector(start.X, start.Y, start.Z - 25.0f);
		FVector tempEnd = FVector(start.X, start.Y, start.Z + 25.0f);

		DrawDebugCylinder(ai->GetWorld(), tempStart, tempEnd, PatrolRadius, 40, FColor::Blue, false, 5.0f);
	}

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);


	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	CheckNull(blackboard);

	FVector goal = blackboard->GetValueAsVector(PatrolKey);
	EPathFollowingRequestResult::Type result = controller->MoveToLocation(goal, AcceptanceRadius, false);


	switch (result)
	{
	case EPathFollowingRequestResult::Failed:
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		break;

	case EPathFollowingRequestResult::AlreadyAtGoal:
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	break;
	}
}
