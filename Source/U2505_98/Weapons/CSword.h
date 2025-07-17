#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeaponStructures.h"
#include "IDamagable.h"
#include "CSword.generated.h"


UCLASS()
class U2505_98_API ACSword 
	: public AActor
	, public IIDamagable
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
	void DoAction();
	void Begin_DoAction();
	void End_DoAction();

public:
	void Begin_Combo();
	void End_Combo();

	void Begin_Collision();
	void End_Collision();

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

private:
	class ACharacter* OwnerCharacter;

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
};
