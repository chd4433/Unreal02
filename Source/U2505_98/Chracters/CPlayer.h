#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapons/ISword.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

UCLASS()
class U2505_98_API ACPlayer 
	: public ACharacter
	, public IISword
	, public IGenericTeamAgentInterface
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
	class UStaticMeshComponent* Sworldsheath;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ACSword> SwordClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<class UCUserWidget_Player> UI_PlayerClass;

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;
	FGenericTeamId GetGenericTeamId() const override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveForward(float InValue);
	void OnMoveRight(float InValue);
	void OnHorizontalLook(float InValue);
	void OnVerticalLook(float InValue);

private:
	void OnRun();
	void OffRun();

private:
	void OnSword() override;
	void OnDoAction() override;

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

private:
	class ACSword* Sword;

private:
	class UCUserWidget_Player* UI_Player;
};
