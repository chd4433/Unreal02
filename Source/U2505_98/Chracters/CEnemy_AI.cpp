#include "Chracters/CEnemy_AI.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "CAIController.h"
#include "CAIStructures.h"
#include "Weapons/CSword.h"
#include "Weapons/CShield.h"
#include "CAIGroup.h"
#include "CPlayer.h"
#include "Widgets/CUserWidget_Enemy.h"

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

void ACEnemy_AI::SetPlayerTarget(ACPlayer* InPlayer)
{
	ACAIController* controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsObject("GroupTarget", Cast<ACharacter>(InPlayer));
}

void ACEnemy_AI::SetWanderTarget(ACPlayer* InPlayer)
{
	ACAIController* controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsObject("WanderTarget", Cast<ACharacter>(InPlayer));
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

	FHelpers::GetClass<ACShield>(&ShieldClass, "/Script/Engine.Blueprint'/Game/Weapons/BP_CShield.BP_CShield_C'");


	FHelpers::GetClass<AController>(&AIControllerClass, "/Script/Engine.Blueprint'/Game/Enemy/BP_CAIController.BP_CAIController_C'");

	FHelpers::GetAsset<UBehaviorTree>(&BehaviorTree, "/Script/AIModule.BehaviorTree'/Game/Enemy/BT_Enemy_AI.BT_Enemy_AI'");

	FHelpers::GetAsset<UAnimMontage>(&ShieldMontage, "/Script/Engine.AnimMontage'/Game/Montages/Shield/Shield_Idle_Montage.Shield_Idle_Montage'");

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

	Shield = GetWorld()->SpawnActor<ACShield>(ShieldClass, transform, params);
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

float ACEnemy_AI::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = ACharacter::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (!!Shield)
	{
		ACSword* sword = Cast<ACSword>(DamageCauser);
		if (!!sword)
		{
			if (Shield->CheckAttackerSword(sword))
			{
				damage -= Shield->ShieldHealth();
				if (damage <= 0.0f)
					damage = 0.0f;
			}

		}
	}

	Health -= damage;
	Health = FMath::Clamp<float>(Health, 0, MaxHealth);
	UI_Enemy->UpdateHealth(Health, MaxHealth);

	if (Health <= 0.0f)
	{
		Dead();

		return 0.0f;
	}


	ACharacter* attacker = Cast<ACharacter>(EventInstigator->GetPawn());

	Damaged((FDamagedDataEvent*)&DamageEvent, attacker);

	return damage;
}

void ACEnemy_AI::Damaged(FDamagedDataEvent* InEvent, ACharacter* InAttacker)
{
	ACAIController* controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsEnum("AIState", (uint8)EAIStateType::Damaged);
	ACEnemy_AI* ai_attacker = Cast<ACEnemy_AI>(InAttacker);
	ACPlayer* player_attacker = Cast<ACPlayer>(InAttacker);

	if (!!GroupManager && bFirstHitted == false)
	{
		bFirstHitted = true;
		
		//Set Group Fighting
		if (ai_attacker && ai_attacker->GetAiGroupManager() && !(GroupManager->GetFightingGroup()) && !(ai_attacker->GetAiGroupManager()->GetFightingGroup()))
		{
			GroupManager->GoToLocation_AllEnemies(GetActorLocation(), Group_State::GroupFighting);

			ai_attacker->SetFirstHitted(true);
			ai_attacker->GetAiGroupManager()->GoToLocation_AllEnemies(ai_attacker->GetActorLocation(), Group_State::GroupFighting);

			SetCountEnemy(ai_attacker);
			ai_attacker->SetCountEnemy(this);

			bFinishGoToLocation = true;
			ai_attacker->SetFinishGoToLocation(true);

			GroupManager->SetFightingGroup(ai_attacker->GetAiGroupManager());
			ai_attacker->GetAiGroupManager()->SetFightingGroup(GroupManager);

			GroupManager->SetState(Group_State::GroupFighting);
			ai_attacker->GetAiGroupManager()->SetState(Group_State::GroupFighting);
		}

		//Set Player Fighting
		if (!!player_attacker)
		{
			GroupManager->GoToLocation_AllEnemies(GetActorLocation(), Group_State::PlayerFighting);

			bPlayerAttacker = true;
			SetPlayerTarget(player_attacker);

			GroupManager->SetState(Group_State::PlayerFighting);
			GroupManager->SetPlayerAttacker(player_attacker);
		}
	}

	bShieldHitAnimation = Shield->CompleteShieldAnimation();

	Super::Damaged(InEvent, InAttacker);
}

void ACEnemy_AI::Dead()
{
	ACAIController* controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsEnum("AIState", (uint8)EAIStateType::Wait);
	bDead = true;
	if (!!GroupManager)
	{
		GroupManager->RemoveEnemy(this);
		if (GroupManager->GetState() == Group_State::PlayerFighting)
			GroupManager->SetPlayerAttacker();
	}

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
	Destroy_Shield();
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

bool ACEnemy_AI::Do_shield()
{
	CheckNullResult(Shield, false);

	return Shield->DoShield();
}

void ACEnemy_AI::Begin_shielded()
{
	CheckNull(Shield);

	PlayAnimMontage(ShieldMontage, ShieldMontage_PlayRate);
	Shield->Begin_shielded();
}

void ACEnemy_AI::End_shielded()
{
	CheckNull(Shield);

	Shield->End_shielded();
}

void ACEnemy_AI::Destroy_Shield()
{
	CheckNull(Shield);

	Shield->Destroy_Shield();
	Shield = nullptr;
}

void ACEnemy_AI::Damaged_State()
{
	ACAIController* controller = GetController<ACAIController>();
	UBlackboardComponent* blackboard = controller->GetBlackboardComponent();
	blackboard->SetValueAsEnum("AIState", (uint8)EAIStateType::Damaged);
}

void ACEnemy_AI::SetActionState()
{
	int32 RandomValue = FMath::RandRange(0, 99);

	if (RandomValue < 70)
	{
		Action = Action_State::Attack;   
	}
	else
	{
		Action = Action_State::Shield;   
	}
}
