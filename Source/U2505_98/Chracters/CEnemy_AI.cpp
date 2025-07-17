#include "Chracters/CEnemy_AI.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CAIController.h"
#include "CAIStructures.h"
#include "Weapons/CSword.h"

ACEnemy_AI::ACEnemy_AI()
{
	FHelpers::GetClass<ACSword>(&SwordClass, "/Script/Engine.Blueprint'/Game/Weapons/BP_CSword.BP_CSword_C'");
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

void ACEnemy_AI::Damaged(FDamagedDataEvent* InEvent, ACharacter* InAttacker)
{
	ACAIController* controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsEnum("AIState", (uint8)EAIStateType::Damaged);

	Super::Damaged(InEvent, InAttacker);
}

void ACEnemy_AI::End_Damaged()
{
	Super::End_Damaged();

	ACAIController* controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsEnum("AIState", (uint8)EAIStateType::Wait);
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
