#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeaponStructures.h"
#include "CSword.generated.h"


enum class ESwordAttackType : uint8
{
	BasicAttack_First = 0, BasicAttack_Second, BasicAttack_Third,UpperAttack, UpperCombo_First, UpperCombo_Second, UpperCombo_Third, Max,
};


UCLASS()
class U2505_98_API ACSword 
	: public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	class UDataTable* WeaponDataTable;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	class UDataTable* DamagedDataTable;


private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;

public:
	FORCEINLINE bool IsEquipped() { return bEquipped; }
	FORCEINLINE bool IsEquipping() { return bEquipping; }
	FORCEINLINE bool IsAttacking() { return bAttacking; }

public:
	void SettingbyType(ACharacter* InCharacter);

public:
	bool IsAttached_Hand();

public:	
	ACSword();

public:
	void OnEquip();

private:
	void Equip();
	void Unequip();

public:
	void Begin_Equip();
	void End_Equip();

public:
	void DoAction(ESwordAttackType InType = ESwordAttackType::BasicAttack_First);
	void Begin_DoAction();
	void End_DoAction();

public:
	void Begin_Combo();
	void End_Combo();

	void Begin_Collision();
	void End_Collision();
	void Destroy_Sword();

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

private:
	class ACharacter* OwnerCharacter;

	class IGenericTeamAgentInterface* TeamIdOwnerCharacter;

private:
	bool bEquipped;
	bool bEquipping;

private:
	bool bEnable;
	bool bExist;

	bool bAttacking;
	int32 Index;


private:
	FWeaponData EquipData;
	FWeaponData UnequipData;

	TArray<FWeaponData> ActionDatas;
	TArray<FDamagedData> DamagedDatas;

private:
	TArray<class IIDamagable*> Hitted;

private:
	TArray<class ACShield*> Shields;
};
