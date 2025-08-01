#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapons/ISword.h"
#include "Weapons/IShield.h"
#include "GenericTeamAgentInterface.h"
#include "Weapons/IDamagable.h"
#include "CPlayer.generated.h"

UCLASS()
class U2505_98_API ACPlayer 
	: public ACharacter
	, public IISword
	, public IGenericTeamAgentInterface
	, public IIDamagable
	, public IIShield
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Team")
	uint8 TeamID = 1;

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* ExecutionSpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* ExecutionCamera;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Sworldsheath;

	UPROPERTY(VisibleAnywhere)
	class UMotionWarpingComponent* MotionWarping;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ACSword> SwordClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ACShield> ShieldClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UCUserWidget_Player> UI_PlayerClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	class UAnimMontage* DeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	float DeadMontage_PlayRate = 1.0f;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	class UAnimMontage* ExecutionMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	float ExecutionMontage_PlayRate = 1.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	bool bDebug;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance = 100;

public:
	FORCEINLINE void SetbRotation(bool InRotation) {  bRotation = InRotation; }

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:
	FGenericTeamId GetGenericTeamId() const override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE void OnFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void OffFixedCamera() { bFixedCamera = false; }

	FORCEINLINE void SetAttackJump(bool InJump) { bAttackJump = InJump; };
	FORCEINLINE bool GetAttackJump() { return bAttackJump; };

private:
	void OnMoveForward(float InValue);
	void OnMoveRight(float InValue);
	void OnHorizontalLook(float InValue);
	void OnVerticalLook(float InValue);

private:
	void OnRun();
	void OffRun();

private:
	void RightClick();

private:
	void LineTrace();
	void Execution();

public:
	void EndExecution();

public:
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	void Damaged(struct FDamagedDataEvent* InEvent, class ACharacter* InAttacker);

public:
	void End_Damaged();

private:
	void Dead();

public:
	void End_Dead();

private:
	void OnSword() override;
	void OnDoAction() override;
	void UpperAttack();
	void DownAttack();

public:
	bool IsEquipped() override;
	bool IsEquipping() override;
	bool IsAttacking()override;
	bool IsAttached_Hand()override;

	void Begin_Equip() override;
	void End_Equip() override;

	void Begin_DoAction() override;
	void End_DoAction()override;

	void Begin_Combo() override;
	void End_Combo() override;

	void Begin_Collision() override;
	void End_Collision() override;
	void Destroy_Sword() override;

public:
	bool Do_shield() override;
	void Begin_shielded() override;
	void End_shielded() override;
	void Destroy_Shield() override;
private:
	class ACSword* Sword;

private:
	class ACShield* Shield;

private:
	class UCUserWidget_Player* UI_Player;

private:
	float Health;
	bool bCanMove;
	bool bRotation;
	bool bFixedCamera;
	bool bAttackJump;

private:
	class ACEnemy* ExecutionEnemy;
};
