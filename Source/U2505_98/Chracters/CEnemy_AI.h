#pragma once

#include "CoreMinimal.h"
#include "Chracters/CEnemy.h"
#include "Weapons/ISword.h"
#include "GenericTeamAgentInterface.h"
#include "CEnemy_AI.generated.h"


UCLASS()
class U2505_98_API ACEnemy_AI 
	: public ACEnemy
	, public IISword
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

public:
	FGenericTeamId GetGenericTeamId() const override;

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE const FVector& GetStartLocation() { return StartLocation; }
	FORCEINLINE void SetAiGroupManager(class ACAIGroup* InGroupManager) { GroupManager = InGroupManager; }
public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override;

	void Damaged(struct FDamagedDataEvent* InEvent, class ACharacter* InAttacker) override;
	void Dead() override;
public:
	void End_Damaged() override;

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

private:
	class ACSword* Sword;

private:
	FVector StartLocation;

private:
	class ACAIGroup* GroupManager;

};
