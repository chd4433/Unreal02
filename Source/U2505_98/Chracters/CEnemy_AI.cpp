#include "Chracters/CEnemy_AI.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CAIController.h"
#include "CAIStructures.h"
#include "Weapons/CSword.h"
#include "CAIGroup.h"

FGenericTeamId ACEnemy_AI::GetGenericTeamId() const
{
	return FGenericTeamId(TeamID);
}

void ACEnemy_AI::SetCountEnemy(ACEnemy_AI* InEnemy)
{
	CountEnemy = InEnemy;

	ACAIController* controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsObject("GroupTarget", Cast<ACharacter>(CountEnemy));
	//blackboard->SetValueAsObject("Target", CountEnemy);
}

void ACEnemy_AI::InitializeGroupFighting()
{
	bFinishGoToLocation = false;
	bFirstHitted = false;
}

ACEnemy_AI::ACEnemy_AI()
{
	PrimaryActorTick.bCanEverTick = true;

	FHelpers::GetClass<ACSword>(&SwordClass, "/Script/Engine.Blueprint'/Game/Weapons/BP_CSword.BP_CSword_C'");

	FHelpers::GetClass<AController>(&AIControllerClass, "/Script/Engine.Blueprint'/Game/Enemy/BP_CAIController.BP_CAIController_C'");

	FHelpers::GetAsset<UBehaviorTree>(&BehaviorTree, "/Script/AIModule.BehaviorTree'/Game/Enemy/BT_Enemy_AI.BT_Enemy_AI'");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACEnemy_AI::BeginPlay()
{
	Super::BeginPlay();


	FTransform transform;

	FActorSpawnParameters params;
	params.Owner = this;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Sword = GetWorld()->SpawnActor<ACSword>(SwordClass, transform, params);
	Sword->SettingbyType(this);

	StartLocation = GetActorLocation();

	OnSword();

	
}

void ACEnemy_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!!CountEnemy)
	{
		if (CountEnemy->bDead)
			CountEnemy = nullptr;
	}
}

void ACEnemy_AI::Damaged(FDamagedDataEvent* InEvent, ACharacter* InAttacker)
{
	ACAIController* controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsEnum("AIState", (uint8)EAIStateType::Damaged);
	ACEnemy_AI* attacker = Cast<ACEnemy_AI>(InAttacker);

	if (!!GroupManager && bFirstHitted == false)
	{
		bFirstHitted = true;
		GroupManager->GoToLocation_AllEnemies(GetActorLocation());
		//Set Group Fighting
		if (attacker && attacker->GetAiGroupManager() && !(GroupManager->GetFightingGroup()) && !(attacker->GetAiGroupManager()->GetFightingGroup()))
		{
			attacker->SetFirstHitted(true);
			attacker->GetAiGroupManager()->GoToLocation_AllEnemies(attacker->GetActorLocation());

			SetCountEnemy(attacker);
			attacker->SetCountEnemy(this);

			bFinishGoToLocation = true;
			attacker->SetFinishGoToLocation(true);

			GroupManager->SetFightingGroup(attacker->GetAiGroupManager());
			attacker->GetAiGroupManager()->SetFightingGroup(GroupManager);
		}
	}

	Super::Damaged(InEvent, InAttacker);
}

void ACEnemy_AI::Dead()
{
	ACAIController* controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsEnum("AIState", (uint8)EAIStateType::Wait);
	bDead = true;
	if(!!GroupManager)
		GroupManager->RemoveEnemy(this);
	Super::Dead();

}

void ACEnemy_AI::End_Damaged()
{
	Super::End_Damaged();

	ACAIController* controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsEnum("AIState", (uint8)EAIStateType::Wait);
}

void ACEnemy_AI::End_Dead()
{
	Super::End_Dead();

	Destroy_Sword();
}

void ACEnemy_AI::OnSword()
{
	CheckNull(Sword);

	Sword->OnEquip();
}


bool ACEnemy_AI::IsEquipped()
{
	CheckNullResult(Sword, false);

	return Sword->IsEquipped();
}

bool ACEnemy_AI::IsEquipping()
{
	CheckNullResult(Sword, false);

	return Sword->IsEquipping();
}

bool ACEnemy_AI::IsAttacking()
{
	CheckNullResult(Sword, false);

	return Sword->IsAttacking();
}

bool ACEnemy_AI::IsAttached_Hand()
{
	CheckNullResult(Sword, false);

	return Sword->IsAttached_Hand();
}

void ACEnemy_AI::Begin_Equip()
{
	CheckNull(Sword);

	Sword->Begin_Equip();
}

void ACEnemy_AI::End_Equip()
{
	CheckNull(Sword);

	Sword->End_Equip();
}

void ACEnemy_AI::OnDoAction()
{
	CheckNull(Sword);

	Sword->DoAction();
}

void ACEnemy_AI::Begin_DoAction()
{
	CheckNull(Sword);

	Sword->Begin_DoAction();
}

void ACEnemy_AI::End_DoAction()
{
	CheckNull(Sword);

	Sword->End_DoAction();
}

void ACEnemy_AI::Begin_Combo()
{
	CheckNull(Sword);

	Sword->Begin_Combo();
}

void ACEnemy_AI::End_Combo()
{
	CheckNull(Sword);

	Sword->End_Combo();
}

void ACEnemy_AI::Begin_Collision()
{
	CheckNull(Sword);

	Sword->Begin_Collision();
}

void ACEnemy_AI::End_Collision()
{
	CheckNull(Sword);

	Sword->End_Collision();
}

void ACEnemy_AI::Destroy_Sword()
{
	CheckNull(Sword);

	Sword->Destroy_Sword();
	Sword = nullptr;
}
