#pragma once

#include "CoreMinimal.h"
#include "Chracters/CEnemy.h"
#include "Weapons/ISword.h"
#include "Weapons/IShield.h"
#include "GenericTeamAgentInterface.h"
#include "CEnemy_AI.generated.h"

enum class Action_State : uint8
{
	Attack = 0, Shield, Max
};

UCLASS()
class U2505_98_API ACEnemy_AI 
	: public ACEnemy
	, public IISword
	, public IIShield
	, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "AI")
	uint8 TeamID = 2;

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ACSword> SwordClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ACShield> ShieldClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	class UAnimMontage* ShieldMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	float ShieldMontage_PlayRate = 1.0f;

public:
	FGenericTeamId GetGenericTeamId() const override;

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE const FVector& GetStartLocation() { return StartLocation; }

	FORCEINLINE class ACAIGroup* GetAiGroupManager() { return GroupManager; }
	FORCEINLINE void SetAiGroupManager(class ACAIGroup* InGroupManager) { GroupManager = InGroupManager; }
	
	FORCEINLINE const bool GetDead() { return bDead; }

	FORCEINLINE const bool GetFirstHitted() { return bFirstHitted; }
	FORCEINLINE void  SetFirstHitted(bool InBool) { bFirstHitted = InBool; }

	FORCEINLINE const bool GetFinishGoToLocation() { return bFinishGoToLocation; }
	FORCEINLINE void SetFinishGoToLocation(bool InFinish) {  bFinishGoToLocation = InFinish; }
	
	FORCEINLINE const ACEnemy_AI* GetCountEnemy() { return CountEnemy; }
	void SetCountEnemy(ACEnemy_AI* InEnemy);
	void SetPlayerTarget(class ACPlayer* InPlayer);
	void SetWanderTarget(class ACPlayer* InPlayer);

	FORCEINLINE  void SetPlayerAttacker(bool InBool) { bPlayerAttacker = InBool; }
	FORCEINLINE  const bool GetPlayerAttacker() { return bPlayerAttacker; }

	FORCEINLINE  const bool GetbShieldHitAnimation() { return bShieldHitAnimation; }

public:
	void InitializeGroupFighting();

public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override;
private:
	void Tick(float DeltaTime) override;

protected:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Damaged(struct FDamagedDataEvent* InEvent, class ACharacter* InAttacker) override;
	void Dead() override;
public:
	void End_Damaged() override;
	void End_Dead()override;
public:
	void OnSword() override;

public:
	bool IsEquipped() override;
	bool IsEquipping()override;
	bool IsAttacking() override;
	bool IsAttached_Hand() override;

	void Begin_Equip()override;
	void End_Equip() override;

	void OnDoAction() override;
	void Begin_DoAction()override;
	void End_DoAction() override;

	void Begin_Combo() override;
	void End_Combo() override;

	void Begin_Collision()override;
	void End_Collision() override;

	void Destroy_Sword() override;

public:
	bool Do_shield() override;
	void Begin_shielded() override;
	void End_shielded() override;

	void Destroy_Shield() override;

public:
	void Damaged_State();

public:
	void SetActionState();
	FORCEINLINE const Action_State GetActionState() { return Action; }

private:
	class ACSword* Sword;

public:
	class ACShield* Shield;

private:
	FVector StartLocation;

private:
	class ACAIGroup* GroupManager;
	ACEnemy_AI* CountEnemy;
	bool bFinishGoToLocation;
	bool bFirstHitted;
	bool bPlayerAttacker;

private:
	bool bDead;

private:
	bool bShieldHitAnimation;

private:
	Action_State Action;
};
